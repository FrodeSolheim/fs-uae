#!/bin/bash

# Opens a new Bash shell and runs `source env/all.src` in it.

# Make a dummy function to be able to have local variables.
__dummy_func() {
    # Helper variable pointing to the env scripts directory.
    local env_path="$(realpath "$(dirname "$BASH_SOURCE")")"
    # Make sure the resulting variable is not empty due to some error. If it is, abort.
    test -z "$env_path" && return

    # Run the sub shell.
    # Note the use of `exec`. It doesn't change much, but it prevents us from creating one
    #   extra unnecessary sub-shell. Without it, `SHLVL` (shell nesting depth) would increase by 2 instead of the optimal 1.
    # Note the use of `--init-file ...`. An alternative is `source ...; exec bash`, but the problem with it is
    #   that bash would run some init scripts, overriding our custom value of `PS1` and who knows what else.
    exec bash --init-file "$env_path/all.src"
}
# Call our dummy funcion.
__dummy_func
# Delete the function.
unset -f __dummy_func
