#ifndef DOSBOX_H
#define DOSBOX_H

#ifndef CH_LIST
#define CH_LIST
#include <list>
#endif

#ifndef CH_STRING
#define CH_STRING
#include <string>
#endif

#define C_FPU 1

#ifdef FSUAE
#include "uae/types.h"
typedef uint8_t Bit8u;
typedef int8_t Bit8s;
typedef uint16_t Bit16u;
typedef int16_t Bit16s;
typedef uint32_t Bit32u;
typedef int32_t Bit32s;
typedef uint64_t Bit64u;
typedef int64_t Bit64s;
#else
typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;
#if defined(_MSC_VER)
typedef unsigned __int64 Bit64u;
typedef signed __int64 Bit64s;
#else
typedef unsigned long long int Bit64u;
typedef signed long long int Bit64s;
#endif
#endif
typedef Bit32u Bitu;
typedef Bit32s Bits;
typedef double Real64;

#define C_UNALIGNED_MEMORY

#define LONGTYPE(a) a##LL

#define INLINE __inline

#define GCC_ATTRIBUTE(x)

#include "logging.h"

extern void E_Exit(const char*,...);
extern void DOSBOX_RunMachine(void);

#define GCC_UNLIKELY(x) x
#define GCC_LIKELY(x) x

#define MAPPER_AddHandler(x1,x2,x3,x4,x5)

class CommandLine {
public:
	CommandLine(int argc, char const * const argv[]);
	CommandLine(char const * const name, char const * const cmdline);
	const char * GetFileName() { return file_name.c_str(); }

	bool FindExist(char const * const name, bool remove = false);
	bool FindHex(char const * const name, int & value, bool remove = false);
	bool FindInt(char const * const name, int & value, bool remove = false);
	bool FindString(char const * const name, std::string & value, bool remove = false);
	bool FindCommand(unsigned int which, std::string & value);
	bool FindStringBegin(char const * const begin, std::string & value, bool remove = false);
	bool FindStringRemain(char const * const name, std::string & value);
	bool GetStringRemain(std::string & value);
	unsigned int GetCount(void);
	void Shift(unsigned int amount = 1);
	Bit16u Get_arglength();

private:
	typedef std::list<std::string>::iterator cmd_it;
	std::list<std::string> cmds;
	std::string file_name;
	bool FindEntry(char const * const name, cmd_it & it, bool neednext = false);
};

extern int x86_fpu_enabled;

#ifdef UAE

#include "dosbox/setup.h"
#include "dosbox/mem.h"

void cmos_selreg(Bitu port, Bitu val, Bitu iolen);
void cmos_writereg(Bitu port, Bitu val, Bitu iolen);
Bitu cmos_readreg(Bitu port, Bitu iolen);
void CMOS_Init(Section* sec, int);
void CMOS_Destroy(Section* sec);
void CMOS_SetRegister(Bitu regNr, Bit8u val);

void CPU_CheckSegments(void);
Bits CPU_Core_Simple_Trap_Run(void);
void CPU_Core_Simple_Init(void);
void CPU_Core_Normal_Init(void);
void CPU_Core_Full_Init(void);
void CPU_Init(Section*, int, int);
//bool CPU_SwitchTask(Bitu new_tss_selector,TSwitchType tstype,Bitu old_eip);
void CPU_ShutDown(Section*);

void FPU_Init(Section*);

void GFX_SetTitle(Bit32s cycles, Bits frameskip, bool paused);

void KEYBOARD_AddBuffer(Bit8u data);
void KEYBOARD_ClrBuffer(void);
void KEYBOARD_Init(Section* sec);

void MEM_Init(Section * sec);
void MEM_SetVGAHandler(void);
void MEM_ShutDown(Section * sec);

void PAGING_Init(Section * sec);
void PAGING_PageFault(PhysPt lin_addr,Bitu page_addr,Bitu faultcode);

void PIC_Init(Section* sec);
void PIC_Destroy(Section* sec);

void TIMER_Destroy(Section*);
void TIMER_Init(Section* sec);
void TIMER_SetGate2(bool);
bool TIMER_GetGate2(void);

Bit8u *x86_cmos_regs(Bit8u *regs);
Bitu x86_in_keyboard(Bitu port);
void x86_out_keyboard(Bitu port, Bitu val);
bool x86_is_keyboard(void);
void x86_pic_write(Bitu port, Bitu val);
Bitu x86_pic_read(Bitu port);
void x86_timer_write(Bitu port, Bitu val);
Bitu x86_timer_read(Bitu port);
void x86_init_reset(void);
uint8_t x86_get_jumpers(void);
void x86_ack_keyboard(void);
void x86_clearirq(uint8_t irqnum);
void x86_doirq_keyboard(void);
void check_x86_irq(void);

PhysPt SelBase(Bitu sel);
void RemoveEMSPageFrame(void);
void PreparePCJRCartRom(void);

#endif

#endif
