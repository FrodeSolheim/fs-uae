
FIXME: Check if this list is current / up2date

Value: none ("None")
Value: Blizzard1230IV (Blizzard 1230 IV)
Value: Blizzard1260 (Blizzard 1240/1260)
Value: Blizzard2060 (Blizzard 2060)
Value: CyberStormMK1 (CyberStorm MK I)
Value: CyberStormMK2 (CyberStorm MK II)
Value: CyberStormMK3 (CyberStorm MK III)
Value: CyberStormPPC (CyberStorm PPC)
Value: BlizzardPPC (Blizzard PPC)
Value: WarpEngineA4000 (Warp Engine)
Value: TekMagic (Tek Magic)
Value: DKB12x0 (DKB 1230/1240)
Value: FusionForty (Fusion Forty)
Value: A3001SI (GVP A3001 Series I)
Value: A3001SII (GVP A3001 Series II)

Code:
    if c.uae_cpuboard_type.explicit:
        value = c.uae_cpuboard_type.explicit
    elif f.matches(c.accelerator, "0"):
        value = "none"
    elif f.matches(c.accelerator, "blizzard-1230-iv"):
        value = "Blizzard1230IV"
    elif f.matches(c.accelerator, "blizzard-1240"):
        value = "Blizzard1260"
    elif f.matches(c.accelerator, "blizzard-1260"):
        value = "Blizzard1260"
    elif f.matches(c.accelerator, "blizzard-2060"):
        value = "Blizzard2060"
    elif f.matches(c.accelerator, "blizzard-ppc"):
        value = "BlizzardPPC"
    elif f.matches(c.accelerator, "cyberstorm-mk-i"):
        value = "CyberStormMK1"
    elif f.matches(c.accelerator, "cyberstorm-mk-ii"):
        value = "CyberStormMK2"
    elif f.matches(c.accelerator, "cyberstorm-mk-iii"):
        value = "CyberStormMK3"
    elif f.matches(c.accelerator, "cyberstorm-ppc"):
        value = "CyberStormPPC"
    elif f.matches(c.accelerator, "warp-engine-a4000"):
        value = "WarpEngineA4000"
    elif f.matches(c.accelerator, "tek-magic"):
        value = "TekMagic"
    elif f.matches(c.accelerator, "dkb-1230"):
        value = "DKB12x0"
    elif f.matches(c.accelerator, "dkb-1240"):
        value = "DKB12x0"
    elif f.matches(c.accelerator, "fusion-forty"):
        value = "FusionForty"
    else:
        f.fail("Unknown accelerator")
