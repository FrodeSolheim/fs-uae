Type: choice

Value: 0 ("None")
Value: 68881
Value: 68882
Value: 68040
Value: 68060

Code:
    if c.uae_fpu_model.explicit:
        value = c.uae_fpu_model.explicit
    elif c.fpu == "0":
        value = "0"
    elif c.fpu == "68881":
        value = "68881"
    elif c.fpu == "68882":
        value = "68882"
    elif c.fpu == "68040":
        value = "68040"
    elif c.fpu == "68060":
        value = "68060"
    else:
        f.fail("Unknown FPU")
