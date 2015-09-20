Summary: Enable a real time clock (RTC) module.
Type: Choice
Default: auto
Example: MSM6242B
Tags: hardware

Enabling a real time clock (RTC) module allows the emulated Amiga to read
the current time (automatically retrieved from the host system). When you
want to override this option, you may also have to set
[uae_chipset_compatible][] to `-`.

Value: auto ("Auto") - Automatically enable RTC module for some Amiga models.
Value: none ("None") - Real time clock is explicitly disabled.
Value: MSM6242B (Oki MSM6242B) - RTC module in A500/500+/600/1200/2000 models.
Value: RP5C01A (Ricoh RP5C01A) - RTC module in big-box Amigas (A3000, A4000).
Value: MSM6242B_A2000 (A2000 MSM6242B) - Only used in very early A2000
       revisions, not even WB 1.3 supports it.

A real time clock is enabled automatically (`auto`) for the following Amiga
models:

* A500+, A2000 and CDTV defaults to `MSM6242B`.
* A500 defaults to `MSM6242B` only when a memory expansion is enabled.
* A1200 defaults to `MSM6242B` only when fast memory is added.
* A3000 and A4000 models default to `RP5C01A`.

# FIXME: A1200 Blizzard expansions!

Enabling warp mode can cause Workbench to not detect the RTC module
because the read time does not progress as expected (FIXME).

In versions prior to FS-UAE 2.5.19dev, A600 erroneously defaulted to having 
`MSM6242B`, while the correct default is `none` (A stock A600 does not
have any RTC module).

Code:
    # FIXME: Blizzard expansions!
    
    if c.uae_chipset_compatible == "-":
        if c.uae_rtc.explicit:
            value = c.uae_rtc.explicit
            if f.matches(value, "none"):
                value = "none"
            elif f.matches(value, "auto"):
                value = "none"
            elif f.matches(value, "MSM6242B"):
                value = "MSM6242B"
            elif f.matches(value, "MSM6242B_A2000"):
                value = "MSM6242B_A2000"
            elif f.matches(value, "RP5C01A"):
                value = "RP5C01A"
            else:
                f.warning(value + ": invalid value")
                value = "none"
        else:
            value = "none"
    else:
        if c.uae_rtc.explicit and c.uae_rtc.explicit != "auto":
            f.warning("uae_rtc is ignored (compatible chipset enabled)")
        if c.uae_chipset_compatible == "Generic":
            value = "RP5C01A"
        elif c.uae_chipset_compatible == "CDTV":
            value = "MSM6242B"
        elif c.uae_chipset_compatible == "CD32":
            value = "none"
        elif c.uae_chipset_compatible == "A500":
            if int(c.int_bogomem_size):
                value = "MSM6242B"
            elif int(c.int_chipmem_size) > 0x80000:
                value = "MSM6242B"
            elif int(c.int_fastmem_size):
                value = "MSM6242B"
            else:
                value = "none"
        elif c.uae_chipset_compatible == "A500+":
            value = "MSM6242B"
        elif c.uae_chipset_compatible == "A600":
            value = "none"
        elif c.uae_chipset_compatible == "A1000":
            value = "none"
        elif c.uae_chipset_compatible == "A1200":
            if int(c.int_fastmem_size) or int(c.int_z3fastmem_size):
                value = "MSM6242B"
            else:
                value = "none"
        elif c.uae_chipset_compatible == "A2000":
            value = "MSM6242B"
        elif c.uae_chipset_compatible == "A3000":
            value = "RP5C01A"
        elif c.uae_chipset_compatible == "A3000T":
            value = "RP5C01A"
        elif c.uae_chipset_compatible == "A4000":
            value = "RP5C01A"
        elif c.uae_chipset_compatible == "A4000T":
            value = "RP5C01A"
        else:
            f.fail("unknown uae_chipset_compatible")

Test A500:
    > amiga_model = A500
    uae_rtc = MSM6242B

Test A500/512K:
    > amiga_model = A500/512K
    uae_rtc = none

Test A500+:
    > amiga_model = A500+
    uae_rtc = MSM6242B

Test A600:
    > amiga_model = A600
    uae_rtc = none

Test A1000:
    > amiga_model = A1000
    uae_rtc = none

Test A1200:
    > amiga_model = A1200
    uae_rtc = none

Test A1200 + 8MB:
    > amiga_model = A1200
    > fast_memory = 8M
    uae_rtc = MSM6242B

Test A1200 + 8MB Z3:
    > amiga_model = A1200
    > zorro_iii_memory = 8M
    uae_rtc = MSM6242B

Test A3000:
    > amiga_model = A3000
    uae_rtc = RP5C01A

Test A4000:
    > amiga_model = A4000
    uae_rtc = RP5C01A

Test explicit RP5C01A:
    > amiga_model = A500+
    > uae_rtc = RP5C01A
    uae_rtc = RP5C01A

Test explicit RP5C01A with wrong case:
    > uae_rtc = rp5c01a
    uae_rtc = RP5C01A

Test explicit MSM6242B:
    > amiga_model = A1200
    > uae_rtc = MSM6242B
    uae_rtc = MSM6242B

Test explicit MSM6242B with wrong case:
    > uae_rtc = msm6242b
    uae_rtc = MSM6242B

Test invalid value:
    > uae_rtc = msm5200b
    uae_rtc = none
