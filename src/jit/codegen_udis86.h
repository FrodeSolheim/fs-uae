#ifndef UAE_CODEGEN_UDIS86_H
#define UAE_CODEGEN_UDIS86_H

#ifdef WITH_UDIS86

// #define USE_UDIS86

#include <udis86.h>
#if defined(__x86_64__)
#define UD_MODE 64
#else
#define UD_MODE 32
#endif // __x86_64__

// 			write_log("%08x ", i); \
//		for ( ; dSize && (!p[dSize-1] || (0x90 == p[dSize-1])); --dSize) ; /* Find ending */ \

#define UDISFN(udis_func, udis_end) { \
	int dSize = (int)((uintptr_t)(udis_end) - (uintptr_t)(udis_func)); \
	if (dSize > 0) { \
		uint8_t* p = (uint8_t*)(udis_func); \
		jit_log("Disassembling %s (size %u bytes) @ 0x%p:", #udis_func, dSize, p) \
		for (int i = 0; i < dSize; i += 0x10) { \
			for (int j = 0; j < 16; ++j) \
				write_log("%s%02x", 8==j ? "  " : " ", p[i + j]); \
			write_log("\n"); \
		} \
		ud_t ud_obj; \
		ud_init(&ud_obj); \
		ud_set_input_buffer(&ud_obj, p, dSize); \
		ud_set_mode(&ud_obj, UD_MODE); \
		ud_set_syntax(&ud_obj, UD_SYN_INTEL); \
		while (dSize > 0) { \
				dSize -= ud_disassemble(&ud_obj); \
				jit_log("%04llx  %20s  %s", (uae_u64) ud_insn_off(&ud_obj), ud_insn_hex(&ud_obj), ud_insn_asm(&ud_obj)); \
		} \
	} else \
		jit_log("Can't dissassemble %s, start (0x%08lx) is larger than end (0x%08lx)", \
				#udis_func, (uintptr_t)udis_func, (uintptr_t)udis_end); \
}

#endif /* WITH_UDIS86 */

#endif /* UAE_CODEGEN_UDIS86_H */
