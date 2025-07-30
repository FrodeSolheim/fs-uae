#ifndef UAE_X86_H
#define UAE_X86_H

bool a1060_init(struct autoconfig_info *aci);
bool a2088xt_init(struct autoconfig_info *aci);
bool a2088t_init(struct autoconfig_info *aci);
bool a2286_init(struct autoconfig_info *aci);
bool a2386_init(struct autoconfig_info *aci);
bool isa_expansion_init(struct autoconfig_info *aci);
void x86_bridge_sync_change(void);
void x86_update_sound(float);
bool x86_mouse(int port, int x, int y, int z, int b);

#define X86_STATE_INACTIVE 0
#define X86_STATE_STOP 1
#define X86_STATE_ACTIVE 2

int is_x86_cpu(struct uae_prefs*);

typedef void (*X86_INTERRUPT_CALLBACK)(int,bool);

uae_u8 x86_infloppy(int portnum);
void x86_outfloppy(int portnum, uae_u8 v);
void x86_initfloppy(X86_INTERRUPT_CALLBACK);
void x86_floppy_run(void);
uae_u16 floppy_get_raw_data(int*);


// ---------
// used by pcem
void x86_ack_keyboard(void);
uint8_t x86_get_jumpers(void);
void x86_clearirq(uint8_t irqnum);

void portout(uint16_t portnum, uint8_t v);
void portout16(uint16_t portnum, uint16_t value);
void portout32(uint16_t portnum, uint32_t value);

uint8_t portin(uint16_t portnum);
uint16_t portin16(uint16_t portnum);
uint32_t portin32(uint16_t portnum);

// used by qemuvga

void x86_map_lfb(int v);

// ---------

// Used by other parts of code FIXME: remove extra declarations

void x86_xt_ide_bios(struct zfile *z, struct romconfig *rc);
void x86_rt1000_bios(struct zfile *z, struct romconfig *rc);

// ----- ??

void io_sethandler(uint16_t base, int size,
	uint8_t(*inb)(uint16_t addr, void *priv),
	uint16_t(*inw)(uint16_t addr, void *priv),
	uint32_t(*inl)(uint16_t addr, void *priv),
	void(*outb)(uint16_t addr, uint8_t  val, void *priv),
	void(*outw)(uint16_t addr, uint16_t val, void *priv),
	void(*outl)(uint16_t addr, uint32_t val, void *priv),
	void *priv);

void io_removehandler(uint16_t base, int size,
	uint8_t(*inb)(uint16_t addr, void *priv),
	uint16_t(*inw)(uint16_t addr, void *priv),
	uint32_t(*inl)(uint16_t addr, void *priv),
	void(*outb)(uint16_t addr, uint8_t  val, void *priv),
	void(*outw)(uint16_t addr, uint16_t val, void *priv),
	void(*outl)(uint16_t addr, uint32_t val, void *priv),
	void *priv);

// ----

#endif /* UAE_X86_H */
