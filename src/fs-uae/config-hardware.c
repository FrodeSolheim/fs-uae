#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <uae/uae.h>
#include <fs/conf.h>
#include <fs/emu.h>
#include <fs/glib.h>
#include "options.h"
#include "config-common.h"
#include "config-model.h"
#include "config-hardware.h"

static void configure_cpu(void)
{
    const char *uae_cpu_24bit_addressing =
            cfg->cpu_32bit_addressing ? "false" : "true";
    const char *uae_cpu_model = cfg->default_cpu;
    const char *uae_fpu_model = cfg->default_fpu;
    const char *uae_mmu_model = cfg->default_mmu;
    //bool allow_6888x_fpu = false;
    //bool allow_68040_fpu = false;
    //bool allow_68060_fpu = false;

    const char *cpu = fs_config_get_const_string(OPTION_CPU);
    if (cpu == NULL || fs_uae_values_matches(cpu, "auto")) {
        /* Go with the already configured value */
    } else if (fs_uae_values_matches(cpu, "68000")) {
        uae_cpu_24bit_addressing = "true";
        uae_cpu_model = "68000";
        uae_fpu_model = "0";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68010")) {
        uae_cpu_24bit_addressing = "true";
        uae_cpu_model = "68010";
        uae_fpu_model = "0";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68EC020")) {
        uae_cpu_24bit_addressing = "true";
        uae_cpu_model = "68020";
        uae_fpu_model = cfg->default_fpu_noninternal;
        uae_mmu_model = "0";
        //allow_6888x_fpu = true;
    } else if (fs_uae_values_matches(cpu, "68020")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68020";
        uae_fpu_model = cfg->default_fpu_noninternal;
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68EC030")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68030";
        uae_fpu_model = "0";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68030")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68030";
        uae_fpu_model = cfg->default_fpu_noninternal;
        uae_mmu_model = "68030";
    } else if (fs_uae_values_matches(cpu, "68EC040")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68040";
        uae_fpu_model = "0";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68LC040")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68040";
        uae_fpu_model = "0";
        uae_mmu_model = "68040";
    } else if (fs_uae_values_matches(cpu, "68040")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68040";
        uae_fpu_model = "68040";
        uae_mmu_model = "68040";
    } else if (fs_uae_values_matches(cpu, "68040-NOMMU")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68040";
        uae_fpu_model = "68040";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68EC060")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68060";
        uae_fpu_model = "0";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68LC060")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68060";
        uae_fpu_model = "0";
        uae_mmu_model = "68060";
    } else if (fs_uae_values_matches(cpu, "68060-NOMMU")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68060";
        uae_fpu_model = "68060";
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(cpu, "68060")) {
        uae_cpu_24bit_addressing = "false";
        uae_cpu_model = "68060";
        uae_fpu_model = "68060";
        uae_mmu_model = "68060";
    } else {
        fs_emu_warning("Unknown CPU specified");
    }

    const char *fpu = fs_config_get_const_string(OPTION_FPU);
    if (fpu == NULL || fs_uae_values_matches(fpu, "auto")) {
        /* Go with the already configured value */
    } else if (fs_uae_values_matches(fpu, "0")) {
        uae_fpu_model = "0";
    } else if (fs_uae_values_matches(fpu, "68881")) {
        if (fs_uae_values_matches(uae_cpu_model, "68020")
                || fs_uae_values_matches(uae_cpu_model, "68030")) {
            uae_fpu_model = "68881";
        } else {
            fs_emu_warning("68881 FPU must be paired with 68020/68030 CPU\n");
        }
    } else if (fs_uae_values_matches(fpu, "68882")) {
        if (strcmp(uae_cpu_model, "68020") == 0
                || strcmp(uae_cpu_model, "68030") == 0) {
            uae_fpu_model = "68882";
        } else {
            fs_emu_warning("68882 FPU must be paired with 68020/68030 CPU\n");
        }
    } else if (fs_uae_values_matches(fpu, "68040")) {
        if (strcmp(uae_cpu_model, "68040") == 0) {
            uae_fpu_model = "68040";
        } else {
            fs_emu_warning("68040 FPU must be paired with 68040 CPU");
        }
    } else if (fs_uae_values_matches(fpu, "68060")) {
        if (strcmp(uae_cpu_model, "68060") == 0) {
            uae_fpu_model = "68060";
        } else {
            fs_emu_warning("68060 FPU must be paired with 68060 CPU");
        }
    } else {
        fs_emu_warning("Unknown FPU specified");
    }

    const char *mmu = fs_config_get_const_string(OPTION_MMU);
    if (mmu == NULL || fs_uae_values_matches(mmu, "auto")) {
        /* Go with the already configured value */
    } else if (fs_uae_values_matches(mmu, "0")) {
        uae_mmu_model = "0";
    } else if (fs_uae_values_matches(mmu, "68030")) {
        if (strcmp(uae_cpu_model, "68030") == 0) {
            uae_mmu_model = "68030";
        } else {
            fs_emu_warning("68030 MMU must be paired with 68030 CPU");
        }
    } else if (fs_uae_values_matches(mmu, "68040")) {
        if (strcmp(uae_cpu_model, "68040") == 0) {
            uae_mmu_model = "68040";
        } else {
            fs_emu_warning("68040 MMU must be paired with 68040 CPU");
        }
    } else if (fs_uae_values_matches(mmu, "68060")) {
        if (strcmp(uae_cpu_model, "68060") == 0) {
            uae_mmu_model = "68060";
        } else {
            fs_emu_warning("68060 MMU must be paired with 68060 CPU");
        }
    } else {
        fs_emu_warning("Unknown MMU specified");
    }

    if (uae_cpu_model[0]) {
        amiga_set_option("cpu_model", uae_cpu_model);
    }
    if (uae_fpu_model[0]) {
        amiga_set_option("fpu_model", uae_fpu_model);
    }
    if (uae_mmu_model[0]) {
        amiga_set_option("mmu_model", uae_mmu_model);
    }
    if (uae_cpu_24bit_addressing[0]) {
        amiga_set_option("cpu_24bit_addressing", uae_cpu_24bit_addressing);
    }
}

void fs_uae_configure_hardware(void)
{
    configure_cpu();
}
