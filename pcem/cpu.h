#ifndef _CPU_H_
#define _CPU_H_

//extern int cpu;
extern int cpu_manufacturer;

/*808x class CPUs*/
#define CPU_8088 0
#define CPU_8086 1

/*286 class CPUs*/
#define CPU_286 2

/*386 class CPUs*/
#define CPU_386SX  3
#define CPU_386DX  4
#define CPU_486SLC 5
#define CPU_486DLC 6

/*486 class CPUs*/
#define CPU_i486SX  7
#define CPU_Am486SX 8
#define CPU_Cx486S  9
#define CPU_i486DX  10
#define CPU_Am486DX 11
#define CPU_Cx486DX 12
#define CPU_iDX4    13
#define CPU_Cx5x86  14

/*586 class CPUs*/
#define CPU_WINCHIP 15
#define CPU_PENTIUM 16
#define CPU_PENTIUMMMX 17
#define CPU_Cx6x86 	18
#define CPU_Cx6x86MX 	19
#define CPU_Cx6x86L 	20
#define CPU_CxGX1 	21

#define MANU_INTEL 0
#define MANU_AMD   1
#define MANU_CYRIX 2
#define MANU_IDT   3

extern int timing_rr;
extern int timing_mr, timing_mrl;
extern int timing_rm, timing_rml;
extern int timing_mm, timing_mml;
extern int timing_bt, timing_bnt;

extern int timing_int, timing_int_rm, timing_int_v86, timing_int_pm, timing_int_pm_outer;
extern int timing_iret_rm, timing_iret_v86, timing_iret_pm, timing_iret_pm_outer;
extern int timing_call_rm, timing_call_pm, timing_call_pm_gate, timing_call_pm_gate_inner;
extern int timing_retf_rm, timing_retf_pm, timing_retf_pm_outer;
extern int timing_jmp_rm, timing_jmp_pm, timing_jmp_pm_gate;

extern int timing_misaligned;

typedef struct
{
        char name[32];
        int cpu_type;
        int speed;
        int rspeed;
        int multi;
        int pci_speed;
        uint32_t edx_reset;
        uint32_t cpuid_model;
        uint16_t cyrix_id;
        int cpu_flags;
        int mem_read_cycles, mem_write_cycles;
        int cache_read_cycles, cache_write_cycles;
        int atclk_div;
} CPU;

extern CPU cpus_8088[];
extern CPU cpus_8086[];
extern CPU cpus_286[];
extern CPU cpus_i386SX[];
extern CPU cpus_i386DX[];
extern CPU cpus_Am386SX[];
extern CPU cpus_Am386DX[];
extern CPU cpus_486SLC[];
extern CPU cpus_486DLC[];
extern CPU cpus_i486[];
extern CPU cpus_Am486[];
extern CPU cpus_Cx486[];
extern CPU cpus_WinChip[];
extern CPU cpus_Pentium5V[];
extern CPU cpus_PentiumS5[];
extern CPU cpus_Pentium[];
extern CPU cpus_6x86[];

extern CPU cpus_pcjr[];
extern CPU cpus_europc[];
extern CPU cpus_pc1512[];
extern CPU cpus_ibmat[];
extern CPU cpus_ibmxt286[];
extern CPU cpus_ps1_m2011[];
extern CPU cpus_ps2_m30_286[];
extern CPU cpus_acer[];

extern int cpu_iscyrix;
extern int cpu_16bitbus;
extern int cpu_busspeed;
extern int cpu_multi;
/*Cyrix 5x86/6x86 only has data misalignment penalties when crossing 8-byte boundaries*/
extern int cpu_cyrix_alignment;

extern int cpu_hasrdtsc;
extern int cpu_hasMSR;
extern int cpu_hasMMX;
extern int cpu_hasCR4;
extern int cpu_hasVME;
extern int cpu_hasCX8;

#define CR4_TSD  (1 << 2)
#define CR4_DE   (1 << 3)
#define CR4_MCE  (1 << 6)
#define CR4_PCE  (1 << 8)

extern uint64_t cpu_CR4_mask;

#define CPU_SUPPORTS_DYNAREC 1
#define CPU_REQUIRES_DYNAREC 2

extern int cpu_cycles_read, cpu_cycles_read_l, cpu_cycles_write, cpu_cycles_write_l;
extern int cpu_prefetch_cycles, cpu_prefetch_width, cpu_mem_prefetch_cycles, cpu_rom_prefetch_cycles;
extern int cpu_waitstates;
extern int cpu_cache_int_enabled, cpu_cache_ext_enabled;

extern uint64_t tsc;

void cyrix_write(uint16_t addr, uint8_t val, void *priv);
uint8_t cyrix_read(uint16_t addr, void *priv);

extern int is8086;

void cpu_CPUID();
void cpu_RDMSR();
void cpu_WRMSR();

extern int cpu_use_dynarec;

extern int xt_cpu_multi;

extern int isa_cycles;
#define ISA_CYCLES(x) (x * isa_cycles)

void cpu_update_waitstates();
void cpu_set(int);
void cpu_set_edx();
void cpu_set_turbo(int turbo);
int cpu_get_speed();

extern int has_vlb;

#endif
