# uae-slirp-implementation #

Slirp Implementation

Type: Choice
Default: `auto`,
Example:

    uae-slirp-implementation = qemu

Values:

 * auto
 * none
 * builtin
 * qemu

Code:

    if c.uae_slirp_implementation.explicit:
        # FIXME: ok? keep already specified value
        value = c.uae_slirp_implementation.explicit
        # FIXME: check value
    else:
        value = "auto"
