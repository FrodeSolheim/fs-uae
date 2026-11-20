# shellcheck shell=sh
# The host's triplet, in the form the dists and env prefixes are named with (sourced, POSIX sh):
# x86_64-linux-gnu, aarch64-linux-gnu, x86_64-apple-darwin, arm64-apple-darwin, and the compiler's
# own triplet otherwise (for example arm-linux-gnueabihf or x86_64-w64-mingw32). Used by the env
# system (od-fs/build/env) and the dist scripts (od-fs/build/dist).

host_triplet() {
    if [ "$(uname -s)" = Darwin ]; then
        # REVIEW: Not run on a Mac. The architecture the shell runs as, so that it is
        # x86_64-apple-darwin under "arch -x86_64" on arm64 (which gcc -dumpmachine may not be).
        case "$(uname -m)" in
            arm64 | aarch64) echo arm64-apple-darwin ;;
            *) echo "$(uname -m)-apple-darwin" ;;
        esac
        return
    fi
    # On Linux, the compiler's target rather than uname -m, which is the kernel's: a 32-bit ARM
    # system can run a 64-bit kernel.
    _host_triplet="$(gcc -dumpmachine)"
    case "$_host_triplet" in
        x86_64-*linux-gnu) echo x86_64-linux-gnu ;;
        aarch64-*linux-gnu) echo aarch64-linux-gnu ;;
        *) echo "$_host_triplet" ;;
    esac
}
