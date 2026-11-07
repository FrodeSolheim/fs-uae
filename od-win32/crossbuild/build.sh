#!/usr/bin/env bash
#
# Cross-compile the native Windows (od-win32) WinUAE from a Linux host.
#
# Two toolchains:
#   clang (default)  clang-cl/lld-link/llvm-rc/llvm-lib, with the MSVC CRT and Windows SDK fetched
#                    by xwin (github.com/Jake-Shadle/xwin)
#   msvc             Microsoft's cl/link/lib/rc running under Wine, installed by msvc-wine
#                    (github.com/mstorsjo/msvc-wine)
# Both use a native nasm and the prebuilt third party libraries from winuaeinclibs.zip. The file
# lists and compiler settings are read from the Visual Studio projects by vcxproj2ninja.py.
#
# Everything downloaded goes into <repo>/.build/winuae-crossbuild (or $WINUAE_CROSSBUILD_DIR), and
# so do the build directories. The host only needs the Ubuntu packages listed in the Dockerfile, or
# just Docker if you use --docker.
#
# Usage: od-win32/crossbuild/build.sh [options] [-- ninja args]
#
#   --toolchain NAME     clang (default) or msvc
#   --docker             build and run inside the Dockerfile's image (a throwaway container, with
#                        the source tree mounted)
#   --config NAME        vcxproj configuration (default: FullRelease)
#   --build-dir DIR      build directory (default: .build/winuae-crossbuild/build-x64-<toolchain>)
#   --sdk-dir DIR        xwin splat directory (default: $WINUAE_XWIN_DIR or
#                        .build/winuae-crossbuild/xwin)
#   --msvc-dir DIR       msvc-wine installation (default: $WINUAE_MSVC_DIR or
#                        .build/winuae-crossbuild/msvc)
#   --dev-dir DIR        winuaeinclibs directory, the C:\dev equivalent (default: $WINUAE_DEV_DIR or
#                        .build/winuae-crossbuild/dev)
#   --accept-license     allow downloading the MSVC CRT / Windows SDK (xwin) or the MSVC toolchain
#                        (msvc-wine), accepting Microsoft's license (also: XWIN_ACCEPT_LICENSE=1)
#   --lto                honour WholeProgramOptimization (ThinLTO with clang, /GL + /LTCG with msvc)
#   --debug-info         produce CodeView debug info and a .pdb
#   --generate-only      write build.ninja but don't build
#
# Environment: CLANG_CL, LLD_LINK, LLVM_LIB, LLVM_RC, NASM, NINJA override the tool names (e.g.
# CLANG_CL=clang-cl-21). XWIN selects an xwin binary instead of the pinned release that is
# downloaded. XWIN_ARGS adds arguments to xwin (for example "--sdk-version 10.0.26100 --crt-version
# 14.44.17.14" to pin the SDK/CRT for reproducible builds), MSVC_WINE_ARGS likewise for msvc-wine's
# vsdownload.py (e.g. "--msvc-version 17.14 --sdk-version 10.0.26100"). WINEPREFIX defaults to
# .build/winuae-crossbuild/wineprefix for --toolchain msvc. With --docker: DOCKER is the docker
# command (default docker), WINUAE_DOCKER_IMAGE the image tag (default winuae-crossbuild) and
# WINUAE_DOCKER_ARGS extra arguments for "docker run".

set -euo pipefail

here=$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
src=$(cd "$here/../.." && pwd)

abspath() { case "$1" in /*|'') printf '%s\n' "$1" ;; *) printf '%s\n' "$PWD/$1" ;; esac; }

root=$(abspath "${WINUAE_CROSSBUILD_DIR:-$src/.build/winuae-crossbuild}")
toolchain=clang
config=FullRelease
platform=x64
build_dir=
sdk_dir=${WINUAE_XWIN_DIR:-$root/xwin}
dev_dir=${WINUAE_DEV_DIR:-$root/dev}
msvc_dir=${WINUAE_MSVC_DIR:-$root/msvc}
accept_license=${XWIN_ACCEPT_LICENSE:-}
gen_args=()
generate_only=
docker=
inclibs_url=${WINUAE_INCLIBS_URL:-https://download.abime.net/winuae/files/b/winuaeinclibs.zip}
xwin_version=0.10.0
# sha256 of xwin-$xwin_version-x86_64-unknown-linux-musl.tar.gz
xwin_sha256=d870eb4b2f390878af6da1ccd3cf321d22fcb72720984853b4be732ae597fc88

while [ $# -gt 0 ]; do
    case "$1" in
        --toolchain) toolchain=$2; shift 2 ;;
        --docker) docker=1; shift ;;
        --config) config=$2; shift 2 ;;
        --build-dir) build_dir=$2; shift 2 ;;
        --sdk-dir) sdk_dir=$2; shift 2 ;;
        --dev-dir) dev_dir=$2; shift 2 ;;
        --msvc-dir) msvc_dir=$2; shift 2 ;;
        --accept-license) accept_license=1; shift ;;
        --lto|--debug-info) gen_args+=("$1"); shift ;;
        --generate-only) generate_only=1; shift ;;
        -h|--help) sed -n '2,/^$/s/^# \{0,1\}//p' "$0"; exit 0 ;;
        --) shift; break ;;
        *) echo "unknown option: $1 (see --help)" >&2; exit 2 ;;
    esac
done
case "$toolchain" in
    clang|msvc) ;;
    *) echo "unknown toolchain: $toolchain (clang or msvc)" >&2; exit 2 ;;
esac
build_dir=$(abspath "${build_dir:-$root/build-$platform-$toolchain}")
sdk_dir=$(abspath "$sdk_dir")
dev_dir=$(abspath "$dev_dir")
msvc_dir=$(abspath "$msvc_dir")

# --- re-run inside a container (--docker) ---------------------------------------------------------
if [ -n "$docker" ]; then
    docker_cmd=${DOCKER:-docker}
    image=${WINUAE_DOCKER_IMAGE:-winuae-crossbuild}
    echo "Building the $image image ..."
    "$docker_cmd" build -q -t "$image" - < "$here/Dockerfile" >/dev/null
    # Everything is mounted at its host path, so the absolute paths in build.ninja and the msvc-wine
    # wrappers are the same inside and outside.
    mkdir -p "$root/home"
    mounted=("$src")
    mounts=(-v "$src:$src")
    for d in "$root" "$sdk_dir" "$msvc_dir" "$dev_dir" "$build_dir"; do
        covered=
        for m in "${mounted[@]}"; do
            case "$d/" in "$m"/*) covered=1 ;; esac
        done
        if [ -z "$covered" ]; then
            mkdir -p "$d"  # or docker creates it, owned by root
            mounted+=("$d")
            mounts+=(-v "$d:$d")
        fi
    done
    tty=()
    [ -t 1 ] && tty=(-t)
    inner=(--toolchain "$toolchain" --config "$config" --build-dir "$build_dir" --sdk-dir "$sdk_dir"
           --msvc-dir "$msvc_dir" --dev-dir "$dev_dir")
    [ -n "$accept_license" ] && inner+=(--accept-license)
    [ -n "$generate_only" ] && inner+=(--generate-only)
    # shellcheck disable=SC2086
    exec "$docker_cmd" run --rm --init ${tty[@]+"${tty[@]}"} --user "$(id -u):$(id -g)" \
        -e HOME="$root/home" -e WINUAE_CROSSBUILD_DIR="$root" -e XWIN_ARGS -e MSVC_WINE_ARGS \
        -e WINUAE_INCLIBS_URL -e NINJA_STATUS "${mounts[@]}" -w "$src" ${WINUAE_DOCKER_ARGS:-} \
        "$image" "$here/build.sh" "${inner[@]}" ${gen_args[@]+"${gen_args[@]}"} -- "$@"
fi

need_license() {
    if [ -z "$accept_license" ]; then
        echo "error: downloading $1 requires accepting Microsoft's license" >&2
        echo "(https://go.microsoft.com/fwlink/?LinkId=2086102)." >&2
        echo "Re-run with --accept-license (or XWIN_ACCEPT_LICENSE=1)." >&2
        exit 1
    fi
}

tmp=
cleanup() { if [ -n "$tmp" ]; then rm -rf "$tmp"; fi; }
trap cleanup EXIT
tmpdir() {
    if [ -z "$tmp" ]; then
        mkdir -p "$root"
        tmp=$(mktemp -d "$root/tmp.XXXXXX")
    fi
}

clang_cl=${CLANG_CL:-clang-cl}
lld_link=${LLD_LINK:-lld-link}
llvm_lib=${LLVM_LIB:-llvm-lib}
llvm_rc=${LLVM_RC:-llvm-rc}
nasm=${NASM:-nasm}
ninja=${NINJA:-ninja}

missing=()
if [ "$toolchain" = clang ]; then
    tools=("$clang_cl" "$lld_link" "$llvm_lib" "$llvm_rc")
    hint="clang lld llvm"
else
    tools=(wine msiextract git)
    hint="wine wine64 msitools winbind git"
fi
for t in "${tools[@]}" "$nasm" "$ninja" python3 curl; do
    command -v "$t" >/dev/null || missing+=("$t")
done
if [ ${#missing[@]} -gt 0 ]; then
    echo "error: missing tools: ${missing[*]}" >&2
    echo "On Debian/Ubuntu: apt install $hint nasm ninja-build python3 curl" >&2
    echo "or build in a container with --docker." >&2
    exit 1
fi

# --- MSVC CRT + Windows SDK (clang) ---------------------------------------------------------------
have_sdk() { [ -f "$sdk_dir/crt/include/vcruntime.h" ] && [ -f "$sdk_dir/crt/include/atlbase.h" ]; }
if [ "$toolchain" = clang ] && ! have_sdk; then
    need_license "the MSVC CRT and Windows SDK"
    xwin=${XWIN:-}
    if [ -z "$xwin" ]; then
        if [ "$(uname -m)" != x86_64 ]; then
            echo "error: no pinned xwin binary for $(uname -m); set XWIN to an xwin executable" >&2
            exit 1
        fi
        name=xwin-$xwin_version-x86_64-unknown-linux-musl
        xwin=$root/tools/$name/xwin
        if [ ! -x "$xwin" ]; then
            echo "Fetching xwin $xwin_version ..."
            tmpdir
            curl -fSL --retry 3 -o "$tmp/$name.tar.gz" \
                "https://github.com/Jake-Shadle/xwin/releases/download/$xwin_version/$name.tar.gz"
            echo "$xwin_sha256  $tmp/$name.tar.gz" | sha256sum -c --quiet
            mkdir -p "$root/tools"
            tar -xzf "$tmp/$name.tar.gz" -C "$root/tools"
        fi
    fi
    echo "Fetching MSVC CRT + Windows SDK into $sdk_dir ..."
    # shellcheck disable=SC2086
    "$xwin" --accept-license --arch x86_64 --include-atl --cache-dir "$root/xwin-download" \
        ${XWIN_ARGS:-} splat --output "$sdk_dir"
    rm -rf "$root/xwin-download"
fi

# --- MSVC toolchain under Wine (msvc) -------------------------------------------------------------
if [ "$toolchain" = msvc ]; then
    if [ ! -x "$msvc_dir/bin/x64/cl" ]; then
        need_license "the MSVC toolchain"
        mw=${MSVC_WINE_SRC:-$root/msvc-wine}
        if [ ! -d "$mw" ]; then
            git clone --depth 1 https://github.com/mstorsjo/msvc-wine "$mw"
        fi
        echo "Fetching MSVC + Windows SDK into $msvc_dir ..."
        # shellcheck disable=SC2086
        "$mw/vsdownload.py" --accept-license --dest "$msvc_dir" --cache "$root/msvc-download" \
            --host-arch x64 --architecture x64 --with-atl yes ${MSVC_WINE_ARGS:-}
        "$mw/install.sh" "$msvc_dir"
        rm -rf "$root/msvc-download"
    fi
    # One wineserver holds the files opened by all parallel cl.exe processes. With the usual soft
    # limit of 1024 open files it runs out, and cl.exe fails at random with "C1083: Cannot open
    # include file". Raise the soft limit to the hard limit (Docker's default is 1024:524288).
    ulimit -n "$(ulimit -Hn)" 2>/dev/null || ulimit -n 1048576 2>/dev/null || true
    if [ "$(ulimit -n)" != unlimited ] && [ "$(ulimit -n)" -lt 65536 ]; then
        echo "warning: only $(ulimit -n) open files allowed; if cl.exe fails with C1083," >&2
        echo "raise the limit (ulimit -Hn) or build with fewer jobs (-- -j4)" >&2
    fi
    export WINEPREFIX=${WINEPREFIX:-$root/wineprefix}
    mkdir -p "$(dirname "$WINEPREFIX")"
    export WINEDEBUG=${WINEDEBUG:--all}
    export WINEDLLOVERRIDES=${WINEDLLOVERRIDES:-mscoree,mshtml=}  # no Mono/Gecko prompts
    if [ ! -f "$WINEPREFIX/system.reg" ]; then
        echo "Creating Wine prefix $WINEPREFIX ..."
        wine wineboot -i >/dev/null 2>&1 || true
    fi
    # A persistent wineserver avoids paying Wine's startup for every cl.exe (Debian/Ubuntu keep
    # wineserver out of PATH)
    for ws in wineserver /usr/lib/x86_64-linux-gnu/wine/wineserver /usr/lib/wine/wineserver64 \
              /usr/lib/wine/wineserver; do
        if command -v "$ws" >/dev/null; then
            "$ws" -p30 || true
            break
        fi
    done
    # msvc-wine's wrappers wait forever if Wine can't start the compiler, so check that it can
    # before handing the build to ninja
    if ! timeout 120 "$msvc_dir/bin/x64/cl" /? >/dev/null 2>&1; then
        echo "error: cannot run cl.exe under Wine (WINEPREFIX=$WINEPREFIX)" >&2
        exit 1
    fi
fi

# --- winuaeinclibs (C:\dev) -----------------------------------------------------------------------
if [ ! -f "$dev_dir/include/zlib.h" ]; then
    echo "Fetching winuaeinclibs.zip into $dev_dir ..."
    tmpdir
    curl -fSL --retry 3 -o "$tmp/winuaeinclibs.zip" "$inclibs_url"
    mkdir -p "$dev_dir"
    python3 -c 'import sys, zipfile; zipfile.ZipFile(sys.argv[1]).extractall(sys.argv[2])' \
        "$tmp/winuaeinclibs.zip" "$dev_dir"
fi

# --- generate + build -----------------------------------------------------------------------------
python3 "$here/vcxproj2ninja.py" \
    --toolchain "$toolchain" --config "$config" --platform "$platform" --build-dir "$build_dir" \
    --xwin "$sdk_dir" --msvc "$msvc_dir" --dev "$dev_dir" --clang-cl "$clang_cl" \
    --lld-link "$lld_link" --llvm-lib "$llvm_lib" --llvm-rc "$llvm_rc" --nasm "$nasm" \
    ${gen_args[@]+"${gen_args[@]}"}

[ -n "$generate_only" ] && exit 0
"$ninja" -C "$build_dir" "$@"

for exe in "$build_dir"/*.exe; do
    if [ -f "$exe" ]; then echo "Built ($toolchain): $exe"; fi
done
