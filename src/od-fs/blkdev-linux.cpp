 /*
  * E-UAE - The portable Amiga Emulator
  *
  * SCSI layer back-end for Linux hosts
  *
  * Copyright 2006 Jochen Becher
  * Copyright 2006 Richard Drummond
  *
  */

#ifdef LINUX

#include "sysconfig.h"
#include "sysdeps.h"

#include "options.h"
#include "blkdev.h"
#include "scsidev.h"
#include "uae/memory.h"
#include "gui.h"

#include <linux/cdrom.h>
#include <sys/ioctl.h>

#undef  DEBUG_ME
#ifdef  DEBUG_ME
# define DEBUG_LOG(fmt,args...)  write_log(_T(fmt) ,##args)
#else
# define DEBUG_LOG(...) do { ; } while (0);
#endif

struct scsidevdata {
    const TCHAR *name;
    int fd;
    int isatapi;
    int cdda_paused;
    struct cd_toc_head toc;
    uae_u8 buf[DEVICE_SCSI_BUFSIZE];
};

#define MAX_SCSI_DRIVES 16
static struct scsidevdata drives[MAX_SCSI_DRIVES];

static uae_u8 *execscsicmd_in (int unitnum, uae_u8 *cmd_data, int cmd_len, int *outlen);

/*
 * this little piece of magic from Toni Wilen is needed to detect
 * ATAPI devices.
 */
static int is_atapi_drive (int unitnum)
{
    static uae_u8 cmd[6] = {0x12, 0, 0, 0, 36, 0}; /* INQUIRY */
    uae_u8 out[36];
    int outlen = sizeof (out);
    uae_u8 *p = execscsicmd_in (unitnum, cmd, sizeof (cmd), &outlen);
    if (!p) {
	DEBUG_LOG ("SCSIDEV: Inquiry command failed; unit %d is not ATAPI drive\n", unitnum);
	return 0;
    }
    if (outlen >= 2 && (p[0] & 31) == 5 && (p[2] & 7) == 0) {
	DEBUG_LOG ("SCSIDEV: unit %d is ATAPI drive\n", unitnum);
	return 1;
    }
    DEBUG_LOG ("SCSIDEV: unit %d is not ATAPI drive\n", unitnum);
    return 0;
}

static int open_bus ( int flags)
{
    int i;
    DEBUG_LOG ("SCSIDEV: open_bus: 0x%x\n", flags);
    for (i = 0; i < MAX_TOTAL_SCSI_DEVICES; i++) {
        drives[i].name = NULL;
        drives[i].fd = -1;
        drives[i].isatapi = 0;
    }
    return 1;
}

static void close_bus (void)
{
    DEBUG_LOG ("SCSIDEV: close_bus\n");
}

static int open_device (int unitnum, const TCHAR *ident, int flags)
{
    int result = 0;
    struct scsidevdata *sdd = &drives[unitnum];

    uae_log("SCSIDEV: unit = %d, name = %s, fd = %d\n", unitnum, ident, sdd->fd);
    if (sdd->fd == -1) {
        sdd->name = ident;
        struct stat sb;
        if (stat(sdd->name, &sb) == -1) {
            uae_log("SCSIDEV: could not stat %s\n", sdd->name);
        }
        else if (S_ISREG(sb.st_mode)) {
            uae_log("SCSIDEV: path is not a device\n");
        }
        else if ((sdd->fd = open (sdd->name, O_RDONLY|O_NONBLOCK)) >= 0) {
            uae_log("SCSIDEV: Successfully opened drive %s\n", sdd->name);
            sdd->isatapi = is_atapi_drive (unitnum);
            result = 1;
        } else {
            uae_log("SCSIDEV: Failed to open drive %s: %d\n", sdd->name, errno);
        }
    } else {
        /* already open */
        uae_log("SCSIDEV: unit %d is already opened as %s.\n", unitnum, sdd->name);
        result = 1;
    }
    return result;
}

static void close_device (int unitnum)
{
    struct scsidevdata *sdd = &drives[unitnum];

    DEBUG_LOG ("SCSIDEV: unit = %d: close_device\n", unitnum);
    if (sdd->fd != -1) {
	close (sdd->fd);
	sdd->fd = -1;
    }
}

static struct scsidevdata *unitisopen(int unitnum)
{
    if (drives[unitnum].fd)
        return &drives[unitnum];
    return NULL;
}

static int media_check (struct scsidevdata *sdd)
{
    if (ioctl (sdd->fd, CDROM_DRIVE_STATUS, CDSL_CURRENT) == CDS_DISC_OK)
        return 1;
    else
        return 0;
}

static int ismedia_ioctl ( int unitnum, int quick)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    if (!sdd)
        return -1;
    return media_check(sdd);
}

static inline void dscsi(struct cdrom_generic_command *cmd)
{
#ifdef DEBUG_ME
    DEBUG_LOG("\t cmd:");
    for (unsigned int i = 0; i < 12; i++) DEBUG_LOG(" %02x", cmd->cmd[i]);
    DEBUG_LOG("\n");
    DEBUG_LOG("\t  buffer: %p\n", cmd->buffer);
    DEBUG_LOG("\t  buflen: %d\n", cmd->buflen);
    DEBUG_LOG("\t  stat  : %d\n", cmd->stat);
    DEBUG_LOG("\t  sense :");
    if (0 && cmd->sense) {
        uae_u8 *bp = (uae_u8 *)cmd->sense;
        for (unsigned int i = 0; i < sizeof(*cmd->sense); i++) {
            DEBUG_LOG(" %02x", bp[i]);
        }
    } else {
        DEBUG_LOG(" %p", cmd->sense);
    }
    DEBUG_LOG("\n");
    DEBUG_LOG("\t  dir   : %d\n", cmd->data_direction);
    DEBUG_LOG("\t  quiet : %d\n", cmd->quiet);
    DEBUG_LOG("\t timeout: %d\n", cmd->timeout);
#endif
}

static uae_u8 *execscsicmd_out (int unitnum, uae_u8 *cmd_data, int cmd_len)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    struct cdrom_generic_command cmd;
    int io_error;

    DEBUG_LOG ("SCSIDEV: unit = %d, execscsicmd_out\n", unitnum);
    if (!sdd) {
	DEBUG_LOG ("SCSIDEV: illegal unit %d.\n", unitnum);
	return 0;
    }
    if (cmd_len > CDROM_PACKET_SIZE) {
	DEBUG_LOG ("SCSIDEV: cmd_len too large (%d)\n", cmd_len);
	return 0;
    }
    memcpy (cmd.cmd, cmd_data, cmd_len);
    cmd.buffer = 0;
    cmd.buflen = 0;
    cmd.stat = 0;
    cmd.sense = 0;
    cmd.data_direction = CGC_DATA_WRITE;
    cmd.quiet = 0;
    cmd.timeout = 80*60;

    gui_flicker_led (LED_CD, unitnum, LED_CD_ACTIVE);

    dscsi(&cmd);
    io_error = ioctl (sdd->fd, CDROM_SEND_PACKET, &cmd);
    dscsi(&cmd);
    DEBUG_LOG ("SCSIDEV: error: %d, stat: %d\n", io_error, cmd.stat);
    if (io_error != 0) {
	DEBUG_LOG ("SCSIDEV: errno: %d, %s\n", errno, strerror (errno));
	return 0;
    }
    return cmd_data;
}

static uae_u8 *execscsicmd_in (int unitnum, uae_u8 *cmd_data, int cmd_len, int *outlen)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    struct cdrom_generic_command cmd;
    int io_error;

    DEBUG_LOG ("SCSIDEV: unit = %d, execscsicmd_in\n", unitnum);
    if (!sdd) {
	DEBUG_LOG ("SCSIDEV: illegal unit %d\n", unitnum);
	return 0;
    }
    if (cmd_len > CDROM_PACKET_SIZE) {
	DEBUG_LOG ("SCSIDEV: cmd_len too large (%d)\n", cmd_len);
	return 0;
    }
    memcpy (cmd.cmd, cmd_data, cmd_len);
    cmd.buffer = sdd->buf;
    cmd.buflen = sizeof (sdd->buf);
    cmd.stat = 0;
    cmd.sense = 0;
    cmd.data_direction = CGC_DATA_READ;
    cmd.quiet = 0;
    cmd.timeout = 80*60;

    gui_flicker_led (LED_CD, unitnum, LED_CD_ACTIVE);

    dscsi(&cmd);
    io_error = ioctl (sdd->fd, CDROM_SEND_PACKET, &cmd);
    dscsi(&cmd);
    DEBUG_LOG ("SCSIDEV: error: %d, stat: %d\n", io_error, cmd.stat);
    if (io_error != 0) {
	DEBUG_LOG ("SCSIDEV: errno: %d, %s\n", errno, strerror (errno));
	return 0;
    }
    if (outlen) {
	*outlen = cmd.buflen;
    }
    return sdd->buf;
}

static int execscsicmd_direct (int unitnum, struct amigascsi *as)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    struct cdrom_generic_command cmd;
    struct request_sense sense;

    uae_u8 *scsi_data         = as->data;
    int     scsi_len          = as->len;
    uae_u8 *scsi_cmd          = as->cmd;
    int     scsi_cmd_len      = as->cmd_len;
    uae_u8  scsi_flags        = as->flags;
    uae_u8  scsi_status       = as->status;
#ifdef DEBUG_ME
    uae_u8 *scsi_sense        = as->sensedata;
#endif
    uae_u16 scsi_sense_len    = as->sense_len;

    int io_error;
    unsigned int senselen;
    int parm;

    uae_u8   *scsi_datap;
    uae_u8   *scsi_datap_org;

    DEBUG_LOG ("SCSIDEV: unit = %d: execscsicmd_direct\n", unitnum);
    DEBUG_LOG ("SCSIDEV: scsi_len = %d, scsi_cmd_len = %d, scsi_sense_len = %d, scsi_flags = %x\n",
	       scsi_len, scsi_cmd_len, scsi_sense_len, scsi_flags);

    if (!sdd) {
	DEBUG_LOG ("SCSIDEV: illegal unit %d.\n", unitnum);
	return -1; /* TODO: better error code */
    }

    if (scsi_cmd_len > CDROM_PACKET_SIZE) {
	DEBUG_LOG ("SCSIDEV: scsi_cmd_len too large (%d)\n", scsi_cmd_len);
	return -5; /* TODO: better code */
    }

    scsi_datap = scsi_datap_org = (scsi_len ? scsi_data : NULL);

    memcpy (cmd.cmd, scsi_cmd, scsi_cmd_len);
    cmd.buffer = scsi_datap;
    cmd.buflen = scsi_len;
    cmd.stat = scsi_status;
    if (sdd->isatapi) {
	scsi_atapi_fixup_pre (cmd.cmd, &scsi_cmd_len, &scsi_datap,
			      &scsi_len, &parm);
    }
    senselen = (scsi_flags & 4) ? 4 : /* SCSIF_OLDAUTOSENSE */
		      (scsi_flags & 2) ? scsi_sense_len : /* SCSIF_AUTOSENSE */
		      0;
    cmd.sense = senselen > 0 ? &sense : 0;
    cmd.data_direction = (scsi_flags & 1) ? CGC_DATA_READ : CGC_DATA_WRITE;
    cmd.quiet = 0;
    cmd.timeout = 80*60;

    gui_flicker_led (LED_CD, unitnum, LED_CD_ACTIVE);

    io_error = ioctl (sdd->fd, CDROM_SEND_PACKET, &cmd);

    as->cmdactual = (io_error >= 0) ? scsi_cmd_len : 0;

    dscsi(&cmd);
    DEBUG_LOG ("SCSIDEV: error: %d, stat: %d\n", io_error, cmd.stat);

    as->status = cmd.stat;
    if (cmd.stat != 0) {
	io_error = 45;  /* HFERR_BadStatus */
	as->actual = 0;
        memcpy(&as->sensedata[0], &sense, as->sense_len);
        as->sactual = as->sense_len;
    } else {
        if (io_error < 0) {
            io_error = 20;
            as->actual = 0;
            as->sactual = 0;
        } else {
            io_error = 0;
            if (sdd->isatapi)
                scsi_atapi_fixup_post (cmd.cmd, scsi_cmd_len, scsi_datap, scsi_datap, &as->len, parm);
            as->actual = as->len - cmd.buflen;
            as->sactual = 0;
        }
    }
    return io_error;
}

static struct device_info *info_device (int unitnum, struct device_info *di, int quick, int session)
{
    struct scsidevdata *sdd = unitisopen(unitnum);

    DEBUG_LOG ("SCSIDEV: unit = %d: info_device\n", unitnum);

    if (sdd) {

	media_check (sdd);

	di->bus		    = 0;
	di->target	    = unitnum;
	di->lun		    = 0;
	di->media_inserted  = media_check (sdd);
	di->write_protected = 1;
	di->bytespersector  = 2048;
	di->cylinders	    = 1;
	di->type	    = INQ_ROMD; /* We only support CD/DVD drives for now */
	di->unitnum	    = unitnum;
	/* TODO: Create a more informative device label */
	sprintf (di->label, "[%s]", sdd->name);
    } else {
	di = 0;
    }

    return di;
}

static int pause_ioctl (int unitnum, int paused)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, pause_ioctl\n", unitnum);
    if (sdd) {
        int old = sdd->cdda_paused;
        if (ioctl(sdd->fd, paused ? CDROMPAUSE : CDROMRESUME, 0) == 0)
            sdd->cdda_paused = paused ? 1 : 0;
        return old;
    }
    return -1;
}

static int stop_ioctl (int unitnum)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, stop_ioctl\n", unitnum);
    if (sdd) {
        int err;
        err = ioctl(sdd->fd, CDROMSTOP, 0);
        return (err >= 0) ? 1 : 0;
    }
    return -1;
}

static uae_u32 volume_ioctl (int unitnum, uae_u16 volume_left, uae_u16 volume_right)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, volume_ioctl\n", unitnum);
    if (sdd) {
        struct cdrom_volctrl cdvol;
        int err;
        uae_u32 old = 0;

        err = ioctl(sdd->fd, CDROMVOLREAD, (long)&cdvol);
        DEBUG_LOG("SCSIDEV: err %d, errno %d\n", err, errno);
        if (err == 0)
            old = ((unsigned int)cdvol.channel1 << 16) | cdvol.channel0;

        cdvol.channel0 = volume_left >> 8;
        cdvol.channel1 = volume_right >> 8;

        ioctl(sdd->fd, CDROMVOLCTRL, (long)&cdvol);
        return old;
    }
    return -1;
}

static int play_ioctl (int unitnum, int startlsn, int endlsn, int scan, play_status_callback statusfunc, play_subchannel_callback subfunc)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, play_ioctl\n", unitnum);
    if (sdd) {
        int err;
        struct cdrom_msf msf;
        startlsn += 150;
        endlsn += 150;
        msf.cdmsf_min0 = (startlsn / 75) / 60;
        msf.cdmsf_sec0 = (startlsn / 75) % 60;
        msf.cdmsf_frame0 = (startlsn % 75);
        msf.cdmsf_min1 = (endlsn / 75) / 60;
        msf.cdmsf_sec1 = (endlsn / 75) % 60;
        msf.cdmsf_frame1 = (endlsn % 75);
        err = ioctl(sdd->fd, CDROMPLAYMSF, (long)&msf);
        DEBUG_LOG("SCSIDEV: err %d, errno %d\n", err, errno);
        return (err >= 0) ? 1 : 0;
    }
    return -1;
}

static int qcode_ioctl (int unitnum, uae_u8 *buf, int sector)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, qcode_ioctl\n", unitnum);
    if (sdd) {
        int err;
        struct cdrom_subchnl qcode;
        qcode.cdsc_format = CDROM_MSF;

        err = ioctl(sdd->fd, CDROMSUBCHNL, &qcode);
        if (err < 0) {
                DEBUG_LOG ("SCSIDEV: unit = %d, qcode_ioctl err %d\n", unitnum,errno);
                return 0;
        }

        buf[0] = 0;
        buf[1] = qcode.cdsc_audiostatus;
        buf[2] = 0;
        buf[3] = 12;
        buf[4] = 0;
        buf[5] = (qcode.cdsc_adr << 4) | qcode.cdsc_ctrl;
        buf[6] = qcode.cdsc_trk;
        buf[7] = qcode.cdsc_ind;
        buf[8] = qcode.cdsc_absaddr.msf.minute;
        buf[9] = qcode.cdsc_absaddr.msf.second;
        buf[10] = qcode.cdsc_absaddr.msf.frame;
        buf[11] = qcode.cdsc_reladdr.msf.minute;
        buf[12] = qcode.cdsc_reladdr.msf.second;
        buf[13] = qcode.cdsc_reladdr.msf.frame;
        DEBUG_LOG("SCSIDEV: QCODE:");
        for (int i = 0; i < 14; i++) DEBUG_LOG(" %02x", buf[i]);
        DEBUG_LOG("\n");
        return 1;
    }
    return -1;
}

void encode_l2 (uae_u8 *p, int address);

static int read_block (struct scsidevdata *sdd, int unitnum, uae_u8 *data, int sector, int size, int sectorsize)
{
	uae_u8 p[4096];
	int ret;
	bool got;

	ret = 0;
	while (size-- > 0) {
		got = false;
		if ((sectorsize == 2048 || sectorsize == 2352)) {
		        int err;
		        struct cdrom_msf *msf = (struct cdrom_msf *)&p[0];
		        msf->cdmsf_min0 = ((sector+150) / 75) / 60;
		        msf->cdmsf_sec0 = ((sector+150) / 75) % 60;
		        msf->cdmsf_frame0 = (sector+150) % 75;
		        err = ioctl(sdd->fd, CDROMREADMODE1, (long)msf);
			if (err >= 0) {
				if (sectorsize == 2352) {
					memset (data, 0, 16);
					memcpy (data + 16, p, 2048);
					encode_l2 (data, sector + 150);
					sector++;
					data += sectorsize;
					ret += sectorsize;
				} else if (sectorsize == 2048) {
					memcpy (data, p, 2048);
					sector++;
					data += sectorsize;
					ret += sectorsize;
				}
				got = true;
			} else {
			    DEBUG_LOG("SCSIDEV: CDROMREADMODE1: errno %d\n", errno);
                        }
		}
		if (!got) {
		        int err;
		        struct cdrom_msf *msf = (struct cdrom_msf *)&p[0];
		        msf->cdmsf_min0 = ((sector+150) / 75) / 60;
		        msf->cdmsf_sec0 = ((sector+150) / 75) % 60;
		        msf->cdmsf_frame0 = (sector+150) % 75;
			err = ioctl(sdd->fd, CDROMREADRAW, (long)msf);
			if (err >= 0) {
                            if (data) {
                                    if (sectorsize >= 2352) {
                                            memcpy (data, p, 2352);
                                            memset (data + 2352, 0xff, sectorsize - 2352);
                                            data += sectorsize;
                                            ret += sectorsize;
                                    } else {
                                            memcpy (data, p + 16, sectorsize);
                                            data += sectorsize;
                                            ret += sectorsize;
                                    }
                            }
                            got = true;
			} else {
			    DEBUG_LOG("SCSIDEV: CDROMREADRAW: errno %d\n", errno);
                        }
		}
		sector++;
	}
	return ret;
}



static int rawread_ioctl(int unitnum, uae_u8 *data, int sector, int size, int sectorsize, uae_u32 extra)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, rawread_ioctl\n", unitnum);
    if (sdd) {
        int ret = 0;
        uint8_t p[4096];

        DEBUG_LOG ("SCSIDEV: rawread: sector %d, size %d, sectorsize %d, extra 0x%x\n", sector, size, sectorsize, extra);
        gui_flicker_led (LED_CD, unitnum, LED_CD_ACTIVE);

        if (sectorsize > 0) {
            if (sectorsize != 2336 && sectorsize != 2352 && sectorsize != 2048 &&
                sectorsize != 2336 + 96 && sectorsize != 2352 + 96 && sectorsize != 2048 + 96)
                return 0;
            while (size-- > 0) {
                if (!read_block (sdd, unitnum, data, sector, 1, sectorsize))
                    break;
                data += sectorsize;
                ret += sectorsize;
                sector++;
            }
        } else {
            uae_u8 sectortype = extra >> 16;
            uae_u8 cmd9 = extra >> 8;
            // int sync = (cmd9 >> 7) & 1;
            // int headercodes = (cmd9 >> 5) & 3;
            // int userdata = (cmd9 >> 4) & 1;
            // int edcecc = (cmd9 >> 3) & 1;
            int errorfield = (cmd9 >> 1) & 3;
            uae_u8 subs = extra & 7;
            if (subs != 0 && subs != 1 && subs != 2 && subs != 4)
                return -1;
            if (errorfield >= 3)
                return -1;

            if (isaudiotrack (&sdd->toc, sector)) {
                if (sectortype != 0 && sectortype != 1)
                    return -2;

                for (int i = 0; i < size; i++) {
                    uae_u8 *odata = data;
                    int blocksize = errorfield == 0 ? 2352 : (errorfield == 1 ? 2352 + 294 : 2352 + 296);
                    int readblocksize = errorfield == 0 ? 2352 : 2352 + 296;
                    if (!read_block (sdd, unitnum, NULL, sector, 1, readblocksize)) {
                        return ret;
                    }
                    if (subs == 0) {
                        memcpy (data, p, blocksize);
                        data += blocksize;
                    } else if (subs == 4) { // all, de-interleaved
                        memcpy (data, p, blocksize);
                        data += blocksize;
                        sub_to_deinterleaved (p + readblocksize, data);
                        data += SUB_CHANNEL_SIZE;
                    } else if (subs == 2) { // q-only
                        memcpy (data, p, blocksize);
                        data += blocksize;
                        uae_u8 subdata[SUB_CHANNEL_SIZE];
                        sub_to_deinterleaved (p + readblocksize, subdata);
                        memcpy (data, subdata + SUB_ENTRY_SIZE, SUB_ENTRY_SIZE);
                    } else if (subs == 1) { // all, interleaved
                        memcpy (data, p, blocksize);
                        memcpy (data + blocksize, p + readblocksize, SUB_CHANNEL_SIZE);
                        data += blocksize + SUB_CHANNEL_SIZE;
                    }
                    ret += data - odata;
                    sector++;
                }
            }
        }
        return ret;
    }

    return -1;
}

static int read_ioctl (int unitnum, uae_u8 *data, int sector, int size)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, read_ioctl sector %d, size %d\n", unitnum, sector, size);
    if (sdd) {
        int err;
        int ret = 0;

        lseek(sdd->fd, sector * 2048, SEEK_SET);
        ret = read(sdd->fd, data, size);
        return ret;

        while (size > 0) {
            struct cdrom_read cdr;
            int tocopy = (size > 2048) ? 2048 : size;
            cdr.cdread_lba = sector;
            cdr.cdread_bufaddr = (char *)data;
            cdr.cdread_buflen = 2048;
            err = ioctl(sdd->fd, CDROMREADMODE1, (long)&cdr);
            if (err < 0)
                return ret;
            sector++;
            size -= tocopy;
            data += tocopy;
        }

        return ret;
    }

    return -1;
}

static int write_ioctl (int unitnum,  uae_u8 *data, int sector, int size)
{
    DEBUG_LOG ("SCSIDEV: unit = %d, write_ioctl not implemented\n", unitnum);
    return -1;
}


static int toc_ioctl (int unitnum, struct cd_toc_head *tocout)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, toc_ioctl\n", unitnum);
    if (sdd) {
        int err;
        struct cd_toc_head *th = &sdd->toc;

        struct cdrom_tochdr hdr;
        err = ioctl(sdd->fd, CDROMREADTOCHDR, (long)&hdr);
        if (err >= 0) {
            int i;

            memset(th, 0, sizeof(*th));
            th->first_track = hdr.cdth_trk0;
            th->first_track_offset = 0;
            th->last_track = hdr.cdth_trk1;
            th->tracks = th->last_track - th->first_track + 1;
            th->points = 0;
                
            struct cd_toc *t = &th->toc[0];

            t->adr = 1;
            t->point = 0xa0;
            t->track = th->first_track;
            th->points++;
            t++;

            for (i = 0; i < th->tracks; i++) {
                struct cdrom_tocentry te;
                te.cdte_track = th->first_track + i;
                te.cdte_format = CDROM_LBA;
                err = ioctl(sdd->fd, CDROMREADTOCENTRY, &te);
                if (err < 0) {
                    DEBUG_LOG("SCSIDEV: errno %d\n", errno);
                    break;
                }
                t->adr = te.cdte_adr;
                t->control = te.cdte_ctrl;
                t->paddress = te.cdte_addr.lba;
                t->point = t->track = th->first_track + i;

                th->points++;
                t++;
            }

            th->lastaddress = (t-1)->paddress;
            th->last_track_offset = th->tracks;

            t->adr = 1;
            t->point = 0xa1;
            t->track = th->last_track;
            t++;
            th->points++;

            t->adr = 1;
            t->point = 0xa2;
            t->paddress = th->lastaddress;
            t++;
            th->points++;

#ifdef DEBUG_ME
            DEBUG_LOG("th: first_track %d (%d), last_track %d (%d)\n",
                    th->first_track, th->first_track_offset,
                    th->last_track, th->last_track_offset);
            DEBUG_LOG("    firstaddress %d, lastaddress %d\n",
                    th->firstaddress, th->lastaddress);
            DEBUG_LOG("    tracks %d, points %d\n",
                    th->tracks, th->points);
            for (i = 0; i < th->points; i++) {
                struct cd_toc *t = &th->toc[i];
                DEBUG_LOG("%d: %02x track %d adr 0x%x ctrl 0x%x lsn %d\n",
                        i, t->point, t->track, t->adr, t->control, t->paddress);
            }
#endif
            memcpy(tocout, th, sizeof (struct cd_toc_head));
            return 1;
        }
    }
    return -1;
}

static int check_isatapi_ioctl (int unitnum)
{
    struct scsidevdata *sdd = unitisopen(unitnum);
    DEBUG_LOG ("SCSIDEV: unit = %d, check_isatapi_ioctl\n", unitnum);
    if (sdd) {
        return sdd->isatapi;
    }
    return 0;
}

struct device_functions devicefunc_scsi_linux_raw = {
    _T("SPTI"),
    open_bus, /* open_bus_func     openbus; */
    close_bus, /* close_bus_func    closebus; */
    open_device, /* open_device_func  opendev; */
    close_device, /* close_device_func closedev; */
    info_device, /* info_device_func  info; */

    execscsicmd_out, /* execscsicmd_out_func    exec_out; */
    execscsicmd_in, /* execscsicmd_in_func     exec_in; */
    execscsicmd_direct, /* execscsicmd_direct_func exec_direct; */

    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    0,

    0,

    ismedia_ioctl, /* ismedia_func ismedia; */
    0,             /* scsiemu_func scsiemu; */
};

struct device_functions devicefunc_scsi_ioctl = {
    _T("IOCTL"),
    open_bus, /* open_bus_func     openbus; */
    close_bus, /* close_bus_func    closebus; */
    open_device, /* open_device_func  opendev; */
    close_device, /* close_device_func closedev; */
    info_device, /* info_device_func  info; */

    execscsicmd_out, /* execscsicmd_out_func    exec_out; */
    execscsicmd_in, /* execscsicmd_in_func     exec_in; */
    execscsicmd_direct, /* execscsicmd_direct_func exec_direct; */

    pause_ioctl, /* pause_func pause; */
    stop_ioctl, /* stop_func  stop; */
    play_ioctl, /* play_func  play; */
    volume_ioctl, /* volume_func volume; */
    qcode_ioctl, /* qcode_func qcode; */
    toc_ioctl, /* toc_func   toc; */
    read_ioctl, /* read_func  read; */
    rawread_ioctl, /* rawread_func  rawread; */
    write_ioctl, /* write_func write; */

    check_isatapi_ioctl, /* isatapi_func isatapi; */

    ismedia_ioctl, /* ismedia_func ismedia; */
    0,             /* scsiemu_func scsiemu; */
};

#else

int uae_od_fs_blkdev_linux_dummy;

#endif
