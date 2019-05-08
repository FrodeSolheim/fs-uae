/*
 * QEMU NE2000 emulation
 *
 * Copyright (c) 2003-2004 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "crc32.h"

#include "qemuuaeglue.h"
#include "queue.h"
#include "threaddep/thread.h"

struct NetClientState
{
	struct NE2000State *ne2000state;
	struct pci_board_state *pcistate;
	const struct pci_board *device;
};

struct MACAddr {
	uint8_t a[6];
};

typedef struct NICPeers {
	//NetClientState *ncs[MAX_QUEUE_NUM];
	int32_t queues;
} NICPeers;

typedef struct NICConf {
	MACAddr macaddr;
	NICPeers peers;
	int32_t bootindex;
} NICConf;

#include "ethernet.h"
#include "memory.h"
#include "pci_hw.h"

#define qemu_get_nic_opaque(x) ((x)->ne2000state)

static struct netdriverdata *td;
static void *sysdata;
static uae_u8 *transmitbuffer;
static volatile int transmitlen;

static void ne2000_receive_check(void);

static int getfunc(void *devv, uae_u8 *d, int *len)
{
	struct s2devstruct *dev = (struct s2devstruct*)devv;

	if (transmitlen <= 0)
		return 0;
	if (transmitlen > *len) {
		write_log(_T("NE2000: too large packet transmission attempt %d > %d\n"), transmitlen, *len);
		transmitlen = 0;
		return 0;
	}
	memcpy(d, transmitbuffer, transmitlen);
	*len = transmitlen;
	transmitlen = 0;
	return 1;
}

#define POLYNOMIAL 0x04c11db6
static unsigned compute_mcast_idx(const uint8_t *ep)
{
	uint32_t crc;
	int carry, i, j;
	uint8_t b;

	crc = 0xffffffff;
	for (i = 0; i < 6; i++) {
		b = *ep++;
		for (j = 0; j < 8; j++) {
			carry = ((crc & 0x80000000L) ? 1 : 0) ^ (b & 0x01);
			crc <<= 1;
			b >>= 1;
			if (carry) {
				crc = ((crc ^ POLYNOMIAL) | carry);
			}
		}
	}
	return crc >> 26;
}

//#include "hw/hw.h"
//#include "hw/pci/pci.h"
//#include "net/net.h"
#include "ne2000.h"
//#include "hw/loader.h"
//#include "sysemu/sysemu.h"

/* debug NE2000 card */
//#define DEBUG_NE2000

static NetClientState ncs;
static NE2000State ne2000state;

#define MAX_ETH_FRAME_SIZE 1514

#define E8390_CMD	0x00  /* The command register (for all pages) */
/* Page 0 register offsets. */
#define EN0_CLDALO	0x01	/* Low byte of current local dma addr  RD */
#define EN0_STARTPG	0x01	/* Starting page of ring bfr WR */
#define EN0_CLDAHI	0x02	/* High byte of current local dma addr  RD */
#define EN0_STOPPG	0x02	/* Ending page +1 of ring bfr WR */
#define EN0_BOUNDARY	0x03	/* Boundary page of ring bfr RD WR */
#define EN0_TSR		0x04	/* Transmit status reg RD */
#define EN0_TPSR	0x04	/* Transmit starting page WR */
#define EN0_NCR		0x05	/* Number of collision reg RD */
#define EN0_TCNTLO	0x05	/* Low  byte of tx byte count WR */
#define EN0_FIFO	0x06	/* FIFO RD */
#define EN0_TCNTHI	0x06	/* High byte of tx byte count WR */
#define EN0_ISR		0x07	/* Interrupt status reg RD WR */
#define EN0_CRDALO	0x08	/* low byte of current remote dma address RD */
#define EN0_RSARLO	0x08	/* Remote start address reg 0 */
#define EN0_CRDAHI	0x09	/* high byte, current remote dma address RD */
#define EN0_RSARHI	0x09	/* Remote start address reg 1 */
#define EN0_RCNTLO	0x0a	/* Remote byte count reg WR */
#define EN0_RTL8029ID0	0x0a	/* Realtek ID byte #1 RD */
#define EN0_RCNTHI	0x0b	/* Remote byte count reg WR */
#define EN0_RTL8029ID1	0x0b	/* Realtek ID byte #2 RD */
#define EN0_RSR		0x0c	/* rx status reg RD */
#define EN0_RXCR	0x0c	/* RX configuration reg WR */
#define EN0_TXCR	0x0d	/* TX configuration reg WR */
#define EN0_COUNTER0	0x0d	/* Rcv alignment error counter RD */
#define EN0_DCFG	0x0e	/* Data configuration reg WR */
#define EN0_COUNTER1	0x0e	/* Rcv CRC error counter RD */
#define EN0_IMR		0x0f	/* Interrupt mask reg WR */
#define EN0_COUNTER2	0x0f	/* Rcv missed frame error counter RD */

#define EN1_PHYS        0x11
#define EN1_CURPAG      0x17
#define EN1_MULT        0x18

#define EN2_STARTPG	0x21	/* Starting page of ring bfr RD */
#define EN2_STOPPG	0x22	/* Ending page +1 of ring bfr RD */

#define EN3_CONFIG0	0x33
#define EN3_CONFIG1	0x34
#define EN3_CONFIG2	0x35
#define EN3_CONFIG3	0x36

/*  Register accessed at EN_CMD, the 8390 base addr.  */
#define E8390_STOP	0x01	/* Stop and reset the chip */
#define E8390_START	0x02	/* Start the chip, clear reset */
#define E8390_TRANS	0x04	/* Transmit a frame */
#define E8390_RREAD	0x08	/* Remote read */
#define E8390_RWRITE	0x10	/* Remote write  */
#define E8390_NODMA	0x20	/* Remote DMA */
#define E8390_PAGE0	0x00	/* Select page chip registers */
#define E8390_PAGE1	0x40	/* using the two high-order bits */
#define E8390_PAGE2	0x80	/* Page 3 is invalid. */

/* Bits in EN0_ISR - Interrupt status register */
#define ENISR_RX	0x01	/* Receiver, no error */
#define ENISR_TX	0x02	/* Transmitter, no error */
#define ENISR_RX_ERR	0x04	/* Receiver, with error */
#define ENISR_TX_ERR	0x08	/* Transmitter, with error */
#define ENISR_OVER	0x10	/* Receiver overwrote the ring */
#define ENISR_COUNTERS	0x20	/* Counters need emptying */
#define ENISR_RDC	0x40	/* remote dma complete */
#define ENISR_RESET	0x80	/* Reset completed */
#define ENISR_ALL	0x3f	/* Interrupts we will enable */

/* Bits in received packet status byte and EN0_RSR*/
#define ENRSR_RXOK	0x01	/* Received a good packet */
#define ENRSR_CRC	0x02	/* CRC error */
#define ENRSR_FAE	0x04	/* frame alignment error */
#define ENRSR_FO	0x08	/* FIFO overrun */
#define ENRSR_MPA	0x10	/* missed pkt */
#define ENRSR_PHY	0x20	/* physical/multicast address */
#define ENRSR_DIS	0x40	/* receiver disable. set in monitor mode */
#define ENRSR_DEF	0x80	/* deferring */

/* Transmitted packet status, EN0_TSR. */
#define ENTSR_PTX 0x01	/* Packet transmitted without error */
#define ENTSR_ND  0x02	/* The transmit wasn't deferred. */
#define ENTSR_COL 0x04	/* The transmit collided at least once. */
#define ENTSR_ABT 0x08  /* The transmit collided 16 times, and was deferred. */
#define ENTSR_CRS 0x10	/* The carrier sense was lost. */
#define ENTSR_FU  0x20  /* A "FIFO underrun" occurred during transmit. */
#define ENTSR_CDH 0x40	/* The collision detect "heartbeat" signal was lost. */
#define ENTSR_OWC 0x80  /* There was an out-of-window collision. */

static void ne2000_reset2(NE2000State *s)
{
    int i;

    s->isr = ENISR_RESET;
	s->cmd = 0;
    memcpy(s->mem, &s->c.macaddr, 6);
    s->mem[14] = 0x57;
    s->mem[15] = 0x57;

    /* duplicate prom data */
    for(i = 15;i >= 0; i--) {
        s->mem[2 * i] = s->mem[i];
        s->mem[2 * i + 1] = s->mem[i];
    }
}

static void ne2000_update_irq(NE2000State *s)
{
    int isr;
    isr = (s->isr & s->imr) & 0x7f;
#if defined(DEBUG_NE2000)
    write_log("NE2000: Set IRQ to %d (%02x %02x)\n",
	   isr ? 1 : 0, s->isr, s->imr);
#endif
	s->irq_callback(ncs.pcistate, isr != 0);
//    qemu_set_irq(s->irq, (isr != 0));
}

static int ne2000_buffer_full(NE2000State *s)
{
    int avail, index, boundary;

    index = s->curpag << 8;
    boundary = s->boundary << 8;
    if (index < boundary)
        avail = boundary - index;
    else
        avail = (s->stop - s->start) - (index - boundary);
    if (avail < (MAX_ETH_FRAME_SIZE + 4))
        return 1;
    return 0;
}

static int ne2000_can_receive(NetClientState *nc)
{
    NE2000State *s = qemu_get_nic_opaque(nc);

    if (s->cmd & E8390_STOP)
        return 1;
    return !ne2000_buffer_full(s);
}

#define MIN_BUF_SIZE 60

static ssize_t ne2000_receive(NetClientState *nc, const uint8_t *buf, size_t size_)
{
    NE2000State *s = qemu_get_nic_opaque(nc);
    int size = size_;
    uint8_t *p;
    unsigned int total_len, next, avail, len, index, mcast_idx;
    uint8_t buf1[60];
    static const uint8_t broadcast_macaddr[6] =
        { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

#if defined(DEBUG_NE2000)
    write_log("NE2000: received len=%d\n", size);
#endif

    if (s->cmd & E8390_STOP || ne2000_buffer_full(s))
        return -1;

    /* XXX: check this */
    if (s->rxcr & 0x10) {
        /* promiscuous: receive all */
    } else {
        if (!memcmp(buf,  broadcast_macaddr, 6)) {
            /* broadcast address */
            if (!(s->rxcr & 0x04))
                return size;
        } else if (buf[0] & 0x01) {
            /* multicast */
            if (!(s->rxcr & 0x08))
                return size;
            mcast_idx = compute_mcast_idx(buf);
            if (!(s->mult[mcast_idx >> 3] & (1 << (mcast_idx & 7))))
                return size;
        } else if (s->mem[0] == buf[0] &&
                   s->mem[2] == buf[1] &&
                   s->mem[4] == buf[2] &&
                   s->mem[6] == buf[3] &&
                   s->mem[8] == buf[4] &&
                   s->mem[10] == buf[5]) {
            /* match */
        } else {
            return size;
        }
    }


    /* if too small buffer, then expand it */
    if (size < MIN_BUF_SIZE) {
        memcpy(buf1, buf, size);
        memset(buf1 + size, 0, MIN_BUF_SIZE - size);
        buf = buf1;
        size = MIN_BUF_SIZE;
    }

    index = s->curpag << 8;
    /* 4 bytes for header */
    total_len = size + 4;
    /* address for next packet (4 bytes for CRC) */
    next = index + ((total_len + 4 + 255) & ~0xff);
    if (next >= s->stop)
        next -= (s->stop - s->start);
    /* prepare packet header */
    p = s->mem + index;
    s->rsr = ENRSR_RXOK; /* receive status */
    /* XXX: check this */
    if (buf[0] & 0x01)
        s->rsr |= ENRSR_PHY;
    p[0] = s->rsr;
    p[1] = next >> 8;
    p[2] = total_len;
    p[3] = total_len >> 8;
    index += 4;

    /* write packet data */
    while (size > 0) {
        if (index <= s->stop)
            avail = s->stop - index;
        else
            avail = 0;
        len = size;
        if (len > avail)
            len = avail;
        memcpy(s->mem + index, buf, len);
        buf += len;
        index += len;
        if (index == s->stop)
            index = s->start;
        size -= len;
    }
    s->curpag = next >> 8;

    /* now we can signal we have received something */
    s->isr |= ENISR_RX;
    ne2000_update_irq(s);

    return size_;
}

static void ne2000_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = (NE2000State*)opaque;
    int offset, page, index;

    addr &= 0xf;
	page = s->cmd >> 6;
	offset = addr | (page << 4);
	if (addr == E8390_CMD)
		page = offset = 0;
#ifdef DEBUG_NE2000
    write_log("NE2000: write reg=0x%x val=0x%02x\n", addr, val);
#endif
    if (addr == E8390_CMD) {
        /* control register */
        s->cmd = val;
        if (!(val & E8390_STOP)) { /* START bit makes no sense on RTL8029... */
            s->isr &= ~ENISR_RESET;
            /* test specific case: zero length transfer */
            if ((val & (E8390_RREAD | E8390_RWRITE)) && s->tcnt == 0) {
                s->isr |= ENISR_RDC;
                ne2000_update_irq(s);
            }
            if (val & E8390_TRANS) {
                index = (s->tpsr << 8);
                /* XXX: next 2 lines are a hack to make netware 3.11 work */
                if (index >= NE2000_PMEM_END)
                    index -= NE2000_PMEM_SIZE;
				// must be before ethernet_trigger
				s->tsr = ENTSR_PTX;
				s->isr |= ENISR_TX;
				s->cmd &= ~E8390_TRANS;
				/* fail safe: check range on the transmitted length  */
                if (index + s->tcnt <= NE2000_PMEM_END) {
					transmitbuffer = s->mem + index;
					transmitlen = s->tcnt;
#ifdef DEBUG_NE2000
					write_log("NE2000: %d byte transmit\n", s->tcnt);
#endif
					ethernet_trigger(td, sysdata);
#if 0
					qemu_send_packet(qemu_get_queue(s->nic), s->mem + index, s->tcnt);
#endif
				}
                /* signal end of transfer */
                ne2000_update_irq(s);
            }
        }
    } else {
        page = s->cmd >> 6;
        offset = addr | (page << 4);
        switch(offset) {
        case EN0_STARTPG:
            s->start = val << 8;
            break;
        case EN0_STOPPG:
            s->stop = val << 8;
            break;
        case EN0_BOUNDARY:
            s->boundary = val;
            break;
        case EN0_IMR:
            s->imr = val;
            ne2000_update_irq(s);
            break;
        case EN0_TPSR:
            s->tpsr = val;
            break;
        case EN0_TCNTLO:
            s->tcnt = (s->tcnt & 0xff00) | val;
            break;
        case EN0_TCNTHI:
            s->tcnt = (s->tcnt & 0x00ff) | (val << 8);
            break;
        case EN0_RSARLO:
            s->rsar = (s->rsar & 0xff00) | val;
            break;
        case EN0_RSARHI:
            s->rsar = (s->rsar & 0x00ff) | (val << 8);
            break;
        case EN0_RCNTLO:
            s->rcnt = (s->rcnt & 0xff00) | val;
            break;
        case EN0_RCNTHI:
            s->rcnt = (s->rcnt & 0x00ff) | (val << 8);
            break;
        case EN0_RXCR:
            s->rxcr = val;
            break;
        case EN0_DCFG:
            s->dcfg = val;
            break;
        case EN0_ISR:
            s->isr &= ~(val & 0x7f);
            ne2000_update_irq(s);
            break;
		case EN1_PHYS:
		case EN1_PHYS + 1:
		case EN1_PHYS + 2:
		case EN1_PHYS + 3:
		case EN1_PHYS + 4:
		case EN1_PHYS + 5:
            s->phys[offset - EN1_PHYS] = val;
            break;
        case EN1_CURPAG:
            s->curpag = val;
            break;
		case EN1_MULT:
		case EN1_MULT + 1:
		case EN1_MULT + 2:
		case EN1_MULT + 3:
		case EN1_MULT + 4:
		case EN1_MULT + 5:
		case EN1_MULT + 6:
		case EN1_MULT + 7:
		s->mult[offset - EN1_MULT] = val;
            break;
        }
    }
}

static uint32_t ne2000_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = (NE2000State*)opaque;
    int offset, page, ret;

    addr &= 0xf;
	page = s->cmd >> 6;
	offset = addr | (page << 4);
	if (addr == E8390_CMD) {
        ret = s->cmd;
    } else {
        switch(offset) {
        case EN0_TSR:
            ret = s->tsr;
            break;
        case EN0_BOUNDARY:
            ret = s->boundary;
            break;
        case EN0_ISR:
            ret = s->isr;
            break;
		case EN0_RSARLO:
			ret = s->rsar & 0x00ff;
			break;
		case EN0_RSARHI:
			ret = s->rsar >> 8;
			break;
		case EN1_PHYS:
		case EN1_PHYS + 1:
		case EN1_PHYS + 2:
		case EN1_PHYS + 3:
		case EN1_PHYS + 4:
		case EN1_PHYS + 5:
			ret = s->phys[offset - EN1_PHYS];
            break;
        case EN1_CURPAG:
            ret = s->curpag;
            break;
		case EN1_MULT:
		case EN1_MULT + 1:
		case EN1_MULT + 2:
		case EN1_MULT + 3:
		case EN1_MULT + 4:
		case EN1_MULT + 5:
		case EN1_MULT + 6:
		case EN1_MULT + 7:
			ret = s->mult[offset - EN1_MULT];
            break;
        case EN0_RSR:
            ret = s->rsr;
            break;
        case EN2_STARTPG:
            ret = s->start >> 8;
            break;
        case EN2_STOPPG:
            ret = s->stop >> 8;
            break;
		case EN0_RTL8029ID0:
			ret = 0x50;
			break;
		case EN0_RTL8029ID1:
			ret = 0x43;
			break;
		case EN3_CONFIG0:
			ret = 0;		/* 10baseT media */
			break;
		case EN3_CONFIG2:
			ret = 0x40;		/* 10baseT active */
			break;
		case EN3_CONFIG3:
			ret = 0x40;		/* Full duplex */
			break;
        default:
            ret = 0x00;
            break;
        }
    }
#ifdef DEBUG_NE2000
    write_log("NE2000: read reg=0x%x val=0x%02x\n", offset, ret);
#endif
    return ret;
}

static inline void ne2000_mem_writeb(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        s->mem[addr] = val;
    }
}

static inline void ne2000_mem_writew(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    addr &= ~1; /* XXX: check exact behaviour if not even */
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        *(uint16_t *)(s->mem + addr) = cpu_to_le16(val);
    }
}

static inline void ne2000_mem_writel(NE2000State *s, uint32_t addr,
                                     uint32_t val)
{
    addr &= ~1; /* XXX: check exact behaviour if not even */
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        stl_le_p(s->mem + addr, val);
    }
}

static inline uint32_t ne2000_mem_readb(NE2000State *s, uint32_t addr)
{
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return s->mem[addr];
    } else {
        return 0xff;
    }
}

static inline uint32_t ne2000_mem_readw(NE2000State *s, uint32_t addr)
{
    addr &= ~1; /* XXX: check exact behaviour if not even */
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return le16_to_cpu(*(uint16_t *)(s->mem + addr));
    } else {
        return 0xffff;
    }
}

static inline uint32_t ne2000_mem_readl(NE2000State *s, uint32_t addr)
{
    addr &= ~1; /* XXX: check exact behaviour if not even */
    if (addr < 32 ||
        (addr >= NE2000_PMEM_START && addr < NE2000_MEM_SIZE)) {
        return ldl_le_p(s->mem + addr);
    } else {
        return 0xffffffff;
    }
}

static inline void ne2000_dma_update(NE2000State *s, int len)
{
    s->rsar += len;
    /* wrap */
    /* XXX: check what to do if rsar > stop */
    if (s->rsar == s->stop)
        s->rsar = s->start;

    if (s->rcnt <= len) {
        s->rcnt = 0;
        /* signal end of transfer */
        s->isr |= ENISR_RDC;
        ne2000_update_irq(s);
    } else {
        s->rcnt -= len;
    }
}

static void ne2000_asic_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = (NE2000State*)opaque;

#ifdef DEBUG_NE2000
    write_log("NE2000: asic write val=0x%04x\n", val);
#endif
    if (s->rcnt == 0)
        return;
    if (s->dcfg & 0x01) {
        /* 16 bit access */
		ne2000_mem_writew(s, s->rsar, val);
        ne2000_dma_update(s, 2);
    } else {
        /* 8 bit access */
        ne2000_mem_writeb(s, s->rsar, val);
        ne2000_dma_update(s, 1);
    }
}

static uint32_t ne2000_asic_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = (NE2000State*)opaque;
    int ret;

    if (s->dcfg & 0x01) {
        /* 16 bit access */
        ret = ne2000_mem_readw(s, s->rsar);
        ne2000_dma_update(s, 2);
    } else {
        /* 8 bit access */
        ret = ne2000_mem_readb(s, s->rsar);
        ne2000_dma_update(s, 1);
    }
#ifdef DEBUG_NE2000
    write_log("NE2000: asic read val=0x%04x\n", ret);
#endif
    return ret;
}

static void ne2000_asic_ioport_writel(void *opaque, uint32_t addr, uint32_t val)
{
    NE2000State *s = (NE2000State*)opaque;

#ifdef DEBUG_NE2000
    write_log("NE2000: asic writel val=0x%04x\n", val);
#endif
    if (s->rcnt == 0)
        return;
    /* 32 bit access */
    ne2000_mem_writel(s, s->rsar, val);
    ne2000_dma_update(s, 4);
}

static uint32_t ne2000_asic_ioport_readl(void *opaque, uint32_t addr)
{
    NE2000State *s = (NE2000State*)opaque;
    int ret;

    /* 32 bit access */
    ret = ne2000_mem_readl(s, s->rsar);
    ne2000_dma_update(s, 4);
#ifdef DEBUG_NE2000
    write_log("NE2000: asic readl val=0x%04x\n", ret);
#endif
    return ret;
}

static void ne2000_reset_ioport_write(void *opaque, uint32_t addr, uint32_t val)
{
    /* nothing to do (end of reset pulse) */
}

static uint32_t ne2000_reset_ioport_read(void *opaque, uint32_t addr)
{
    NE2000State *s = (NE2000State*)opaque;
    ne2000_reset2(s);
    return 0;
}

#if 0
static int ne2000_post_load(void* opaque, int version_id)
{
    NE2000State* s = (NE2000State*)opaque;

    if (version_id < 2) {
        s->rxcr = 0x0c;
    }
    return 0;
}

const VMStateDescription vmstate_ne2000 = {
    .name = "ne2000",
    .version_id = 2,
    .minimum_version_id = 0,
    .post_load = ne2000_post_load,
    .fields = (VMStateField[]) {
        VMSTATE_UINT8_V(rxcr, NE2000State, 2),
        VMSTATE_UINT8(cmd, NE2000State),
        VMSTATE_UINT32(start, NE2000State),
        VMSTATE_UINT32(stop, NE2000State),
        VMSTATE_UINT8(boundary, NE2000State),
        VMSTATE_UINT8(tsr, NE2000State),
        VMSTATE_UINT8(tpsr, NE2000State),
        VMSTATE_UINT16(tcnt, NE2000State),
        VMSTATE_UINT16(rcnt, NE2000State),
        VMSTATE_UINT32(rsar, NE2000State),
        VMSTATE_UINT8(rsr, NE2000State),
        VMSTATE_UINT8(isr, NE2000State),
        VMSTATE_UINT8(dcfg, NE2000State),
        VMSTATE_UINT8(imr, NE2000State),
        VMSTATE_BUFFER(phys, NE2000State),
        VMSTATE_UINT8(curpag, NE2000State),
        VMSTATE_BUFFER(mult, NE2000State),
        VMSTATE_UNUSED(4), /* was irq */
        VMSTATE_BUFFER(mem, NE2000State),
        VMSTATE_END_OF_LIST()
    }
};

static const VMStateDescription vmstate_pci_ne2000 = {
    .name = "ne2000",
    .version_id = 3,
    .minimum_version_id = 3,
    .fields = (VMStateField[]) {
        VMSTATE_PCI_DEVICE(dev, PCINE2000State),
        VMSTATE_STRUCT(ne2000, PCINE2000State, 0, vmstate_ne2000, NE2000State),
        VMSTATE_END_OF_LIST()
    }
};
#endif

static uint64_t ne2000_read(void *opaque, hwaddr addr,
                            unsigned size)
{
	uint64_t v;
    NE2000State *s = (NE2000State*)opaque;

    if (addr < 0x10) {
		if (size == 1) {
	        v = ne2000_ioport_read(s, addr);
		} else {
			v = ne2000_ioport_read(s, addr);
			v |= ne2000_ioport_read(s, addr) << 8;
		}
	} else if (addr >= 0x10 && addr <= 0x17) {
        if (size <= 2) {
            v = ne2000_asic_ioport_read(s, addr);
        } else {
            v = ne2000_asic_ioport_readl(s, addr);
        }
    } else if (addr >= 0x18 && addr < 0x20) {
        v = ne2000_reset_ioport_read(s, addr);
    } else {
		v = ((uint64_t)1 << (size * 8)) - 1;
	}
#if defined(DEBUG_NE2000)
	write_log(_T("NE2000_READ %08x=%08x %d\n"), addr, (uae_u32)v, size);
#endif
	return v;
}

static void ne2000_write(void *opaque, hwaddr addr,
                         uint64_t data, unsigned size)
{
    NE2000State *s = (NE2000State*)opaque;

#if defined(DEBUG_NE2000)
	write_log(_T("NE2000_WRITE %08x %08x %d\n"), addr, (uae_u32)data, size);
#endif

	if (addr < 0x10) {
		if (size == 1) {
	        ne2000_ioport_write(s, addr, data);
		} else {
			ne2000_ioport_write(s, addr, data);
			ne2000_ioport_write(s, addr + 1, data >> 8);
		}
	} else if (addr >= 0x10 && addr <= 0x17) {
		if (size <= 2) {
            ne2000_asic_ioport_write(s, addr, data);
        } else {
            ne2000_asic_ioport_writel(s, addr, data);
        }
    } else if (addr >= 0x18 && addr < 0x20) {
        ne2000_reset_ioport_write(s, addr, data);
    }
}

#if 0
static const MemoryRegionOps ne2000_ops = {
    .read = ne2000_read,
    .write = ne2000_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
};

/***********************************************************/
/* PCI NE2000 definitions */

void ne2000_setup_io(NE2000State *s, DeviceState *dev, unsigned size)
{
    memory_region_init_io(&s->io, OBJECT(dev), &ne2000_ops, s, "ne2000", size);
}

static NetClientInfo net_ne2000_info = {
    .type = NET_CLIENT_OPTIONS_KIND_NIC,
    .size = sizeof(NICState),
    .can_receive = ne2000_can_receive,
    .receive = ne2000_receive,
};

static void pci_ne2000_realize(PCIDevice *pci_dev, Error **errp)
{
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s;
    uint8_t *pci_conf;

    pci_conf = d->dev.config;
    pci_conf[PCI_INTERRUPT_PIN] = 1; /* interrupt pin A */

    s = &d->ne2000;
    ne2000_setup_io(s, DEVICE(pci_dev), 0x100);
    pci_register_bar(&d->dev, 0, PCI_BASE_ADDRESS_SPACE_IO, &s->io);
    s->irq = pci_allocate_irq(&d->dev);

    qemu_macaddr_default_if_unset(&s->c.macaddr);
    ne2000_reset(s);

    s->nic = qemu_new_nic(&net_ne2000_info, &s->c,
                          object_get_typename(OBJECT(pci_dev)), pci_dev->qdev.id, s);
    qemu_format_nic_info_str(qemu_get_queue(s->nic), s->c.macaddr.a);
}

static void pci_ne2000_exit(PCIDevice *pci_dev)
{
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s = &d->ne2000;

    qemu_del_nic(s->nic);
    qemu_free_irq(s->irq);
}

static void ne2000_instance_init(Object *obj)
{
    PCIDevice *pci_dev = PCI_DEVICE(obj);
    PCINE2000State *d = DO_UPCAST(PCINE2000State, dev, pci_dev);
    NE2000State *s = &d->ne2000;

    device_add_bootindex_property(obj, &s->c.bootindex,
                                  "bootindex", "/ethernet-phy@0",
                                  &pci_dev->qdev, NULL);
}

static Property ne2000_properties[] = {
    DEFINE_NIC_PROPERTIES(PCINE2000State, ne2000.c),
    DEFINE_PROP_END_OF_LIST(),
};

static void ne2000_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    PCIDeviceClass *k = PCI_DEVICE_CLASS(klass);

    k->realize = pci_ne2000_realize;
    k->exit = pci_ne2000_exit;
    k->romfile = "efi-ne2k_pci.rom",
    k->vendor_id = PCI_VENDOR_ID_REALTEK;
    k->device_id = PCI_DEVICE_ID_REALTEK_8029;
    k->class_id = PCI_CLASS_NETWORK_ETHERNET;
    dc->vmsd = &vmstate_pci_ne2000;
    dc->props = ne2000_properties;
    set_bit(DEVICE_CATEGORY_NETWORK, dc->categories);
}

static const TypeInfo ne2000_info = {
    .name          = "ne2k_pci",
    .parent        = TYPE_PCI_DEVICE,
    .instance_size = sizeof(PCINE2000State),
    .class_init    = ne2000_class_init,
    .instance_init = ne2000_instance_init,
};

static void ne2000_register_types(void)
{
    type_register_static(&ne2000_info);
}

type_init(ne2000_register_types)
#endif

#define MAX_PACKET_SIZE 1600
#define MAX_RECEIVE_BUFFER_INDEX 256
static int receive_buffer_index;
static uae_u8 *receive_buffer;
static volatile int receive_buffer_read, receive_buffer_write;
static int receive_buffer_size[MAX_RECEIVE_BUFFER_INDEX];
static uae_sem_t ne2000_sem;

static void ne2000_receive_check2(void)
{
	if (receive_buffer_read != receive_buffer_write) {
		if (ne2000state.isr & ENISR_RX)
			return;
		if (ne2000_receive(&ncs, receive_buffer + receive_buffer_read * MAX_PACKET_SIZE, receive_buffer_size[receive_buffer_read]) < 0)
			return;
#ifdef DEBUG_NE2000
		write_log("NE2000: %d byte receive accepted (%d %d)\n", receive_buffer_size[receive_buffer_read], receive_buffer_read, receive_buffer_write);
#endif
		receive_buffer_read++;
		receive_buffer_read &= (MAX_RECEIVE_BUFFER_INDEX - 1);
	}
}

static void ne2000_receive_check(void)
{
	if (receive_buffer_read != receive_buffer_write) {
		uae_sem_wait(&ne2000_sem);
		if (receive_buffer_read != receive_buffer_write) {
			uae_sem_post(&ne2000_sem);
			ne2000_receive_check2();
		} else {
			uae_sem_post(&ne2000_sem);
		}
	}
}

static void gotfunc(void *devv, const uae_u8 *databuf, int len)
{
#ifdef DEBUG_NE2000
	write_log("NE2000: %d byte received (%d %d)\n", len, receive_buffer_read, receive_buffer_write);
#endif
	ne2000_receive_check();
	if (len > MAX_PACKET_SIZE) 
		return;
	uae_sem_wait(&ne2000_sem);
	int nextwrite = (receive_buffer_write + 1) & (MAX_RECEIVE_BUFFER_INDEX - 1);
	if (nextwrite == receive_buffer_read) {
		uae_sem_post(&ne2000_sem);
		write_log("NE2000: receive buffer full\n");
		return;
	}
	memcpy(receive_buffer + receive_buffer_write * MAX_PACKET_SIZE, databuf, len);
	receive_buffer_size[receive_buffer_write] = len;
	receive_buffer_write++;
	receive_buffer_write &= (MAX_RECEIVE_BUFFER_INDEX - 1);
	uae_sem_post(&ne2000_sem);
}	

static void ne2000_hsync_handler(struct pci_board_state *pcibs)
{
	ne2000_receive_check();
}

static void REGPARAM2 ne2000_bput(struct pci_board_state *pcibs, uaecptr addr, uae_u32 b)
{
	ne2000_write(ncs.ne2000state, addr, b & 0xff, 1);
}
static void REGPARAM2 ne2000_wput(struct pci_board_state *pcibs, uaecptr addr, uae_u32 b)
{
	ne2000_write(ncs.ne2000state, addr, b & 0xffff, 2);
}
static void REGPARAM2 ne2000_lput(struct pci_board_state *pcibs, uaecptr addr, uae_u32 b)
{
	ne2000_write(ncs.ne2000state, addr, b, 4);
}
static uae_u32 REGPARAM2 ne2000_bget(struct pci_board_state *pcibs, uaecptr addr)
{
	uae_u32 v = ne2000_read(ncs.ne2000state, addr, 1);
	return v;
}
static uae_u32 REGPARAM2 ne2000_wget(struct pci_board_state *pcibs, uaecptr addr)
{
	uae_u32 v = ne2000_read(ncs.ne2000state, addr, 2);
	return v;
}
static uae_u32 REGPARAM2 ne2000_lget(struct pci_board_state *pcibs, uaecptr addr)
{
	uae_u32 v = ne2000_read(ncs.ne2000state, addr, 4);
	return v;
}

static void ne2000_reset(struct pci_board_state *pcibs)
{
	ne2000_reset2(&ne2000state);
	receive_buffer_read = receive_buffer_write = 0;
}

static void ne2000_free(struct pci_board_state *pcibs)
{
	ethernet_close(td, sysdata);
	xfree(sysdata);
	td = NULL;
	sysdata = NULL;
	xfree(receive_buffer);
	receive_buffer = NULL;
	uae_sem_destroy(&ne2000_sem);
}

static bool ne2000_init(struct pci_board_state *pcibs)
{
	ne2000_free(pcibs);
	ncs.device = &ne2000_pci_board;
	ncs.pcistate = pcibs;
	ncs.ne2000state = &ne2000state;
	memset(&ne2000state, 0, sizeof ne2000state);
	ncs.ne2000state->irq_callback = pcibs->irq_callback;

	uae_sem_init(&ne2000_sem, 0, 1);
	if (!receive_buffer) {
		receive_buffer = xcalloc(uae_u8, MAX_PACKET_SIZE * MAX_RECEIVE_BUFFER_INDEX);
	}

	td = NULL;
	uae_u8 *m = ncs.ne2000state->c.macaddr.a;
	const TCHAR *name = currprefs.ne2000pciname[0] ? currprefs.ne2000pciname : currprefs.ne2000pcmcianame; // hack!
	memset(m, 0, 6);
	if (ethernet_enumerate(&td, name)) {
		memcpy(m, td->mac, 6);
		if (!m[0] && !m[1] && !m[2]) {
			m[0] = 0x52;
			m[1] = 0x54;
			m[2] = 0x05;
		}
		write_log(_T("NE2000: '%s' %02X:%02X:%02X:%02X:%02X:%02X\n"),
				  td->name, td->mac[0], td->mac[1], td->mac[2], td->mac[3], td->mac[4], td->mac[5]);
	} else {
		m[0] = 0x52;
		m[1] = 0x54;
		m[2] = 0x05;
		m[3] = 4;
		m[4] = 3;
		m[5] = 2;
		write_log(_T("NE2000: Disconnected mode %02X:%02X:%02X:%02X:%02X:%02X\n"),
				  m[0], m[1], m[2], m[3], m[4], m[5]);
	}

	ne2000_reset(pcibs);

	if (td != NULL) {
		if (!sysdata)
			sysdata = xcalloc(uae_u8, ethernet_getdatalenght(td));
		if (!ethernet_open(td, sysdata, NULL, gotfunc, getfunc, 0)) {
			write_log(_T("NE2000: failed to initialize winpcap driver\n"));
			return false;
		}
	}
	return true;
}


static const struct pci_config ne2000_pci_config =
{
	0x10ec, 0x8029, 0, 0, 0, 0x020000, 0, 0x10ec, 0x8029, 1, 0, 0, { 0x20 | 1, 0, 0, 0, 0, 0, 0 }
};

const struct pci_board ne2000_pci_board =
{
	_T("RTL8029"),
	&ne2000_pci_config, ne2000_init, ne2000_free, ne2000_reset, ne2000_hsync_handler,
	{
		{ ne2000_lget, ne2000_wget, ne2000_bget, ne2000_lput, ne2000_wput, ne2000_bput },
		{ NULL },
		{ NULL },
		{ NULL },
		{ NULL },
		{ NULL },
		{ NULL },
	}
};
