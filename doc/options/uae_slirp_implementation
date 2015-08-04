Summary: Slirp Implementation
Type: Choice
Default: auto
Example: qemu

Value: auto
Value: none
Value: builtin
Value: qemu

Code:
    if c.uae_slirp_implementation.explicit:
        # FIXME: ok? keep already specified value
        value = c.uae_slirp_implementation.explicit
        # FIXME: check value
    else:
        value = "auto"
