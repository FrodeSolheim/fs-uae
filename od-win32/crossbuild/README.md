# Cross-compiling WinUAE (od-win32) from Linux

This builds the native Windows WinUAE (`winuae64.exe`) on a Linux host with one of two toolchains:

- **`clang`** (the default) uses clang-cl and lld-link with the MSVC headers and libraries. It runs
  fully natively and needs no Wine.
- **`msvc`** uses Microsoft's own `cl.exe`, `link.exe` and `rc.exe`, running under Wine via
  [msvc-wine](https://github.com/mstorsjo/msvc-wine). This is the same compiler as a Visual Studio
  build (toolset v145).

The exe is left in the build directory,
`.build/winuae-crossbuild/build-x64-<toolchain>/winuae64.exe`.

## With make

```sh
make -C od-win32/crossbuild
# -> od-win32/crossbuild/winuae64.exe
```

This builds the Docker image, runs an `msvc` build in a container (see below) and copies
`winuae64.exe` into `od-win32/crossbuild/`. Running it accepts Microsoft's license for the
downloaded MSVC toolchain. `make TOOLCHAIN=clang` uses clang instead, and `make clean` removes the
build directory and the copied exe.

## In a container

With only Docker installed on the host:

```sh
od-win32/crossbuild/build.sh --docker --accept-license
od-win32/crossbuild/build.sh --docker --toolchain msvc --accept-license
```

`--docker` builds the image from the [`Dockerfile`](Dockerfile), an Ubuntu 26.04 image with the
toolchain packages. It then runs `build.sh` in a throwaway container (`docker run --rm`) as your own
user. The source tree is mounted at the same path as on the host, so the build directory and the
downloaded tools work both inside and outside the container. `DOCKER=podman` selects another
container runtime, and `WINUAE_DOCKER_ARGS` passes extra options to `docker run`.

## Directly on the host

The packages listed in the Dockerfile are all that is needed:

```sh
sudo apt install clang lld llvm nasm ninja-build python3 curl git ca-certificates \
                 wine wine64 msitools winbind     # the last line only for --toolchain msvc
od-win32/crossbuild/build.sh --accept-license
od-win32/crossbuild/build.sh --toolchain msvc --accept-license
```

## Downloads

Everything that is downloaded goes into `.build/winuae-crossbuild/` in the source tree (or
`$WINUAE_CROSSBUILD_DIR`), next to the build directories. Downloads happen once, on the first build
that needs them.

| Directory | Contents | Toolchain |
| --- | --- | --- |
| `tools/` | [xwin](https://github.com/Jake-Shadle/xwin) release binary (a pinned version, sha256-checked) | clang |
| `xwin/` | MSVC CRT, ATL and Windows SDK, fetched by xwin (about 700 MB) | clang |
| `msvc-wine/`, `msvc/` | msvc-wine checkout and its MSVC + Windows SDK install (1.2 GB download, 4 GB unpacked) | msvc |
| `wineprefix/` | the Wine prefix used to run `cl.exe` | msvc |
| `dev/` | `winuaeinclibs.zip`: the prebuilt third-party headers and libraries that the Visual Studio build expects in `C:\dev` | both |
| `build-x64-clang/`, `build-x64-msvc/` | ninja build directories | |

The MSVC CRT/SDK and the MSVC toolchain fall under Microsoft's license, so they are only downloaded
with `--accept-license` (or `XWIN_ACCEPT_LICENSE=1`).

Later runs are incremental ninja builds. See `build.sh --help` for options: `--toolchain`,
`--docker`, `--config`, `--build-dir`, `--sdk-dir`, `--msvc-dir`, `--dev-dir`, `--lto`,
`--debug-info`, `--generate-only`. Arguments after `--` go to ninja.

## How it works

`vcxproj2ninja.py` reads `winuae_msvc15/winuae_msvc.vcxproj` and `prowizard/prowizard.vcxproj`,
using the configuration mapping in `winuae_msvc.sln`, and writes a `build.ninja`. The Visual Studio
project therefore stays the only file list. Files added or removed there are picked up
automatically, along with per-file defines, include paths, forced includes and `ExcludedFromBuild`.

For the `msvc` toolchain, the vcxproj settings are passed to `cl.exe` and `link.exe` as they are.
`/GL` and `/LTCG` are used only with `--lto`. Unix paths on the command line are translated to
Wine's `Z:` drive by msvc-wine's wrappers, and the generator writes `Z:` paths into response files
itself.

For the `clang` toolchain, MSBuild settings are translated to the equivalent clang-cl, lld-link and
llvm-rc options. A few differences are deliberate:

| MSVC | Cross build | Reason |
| --- | --- | --- |
| `/fp:fast` | clang's default (precise) | clang's `/fp:fast` means `-ffast-math`, which assumes there are no NaNs or infinities. The FPU emulation depends on them. |
| `/std:c++latest` | `/std:c++23preview` | clang's "latest" is C++26, which turns some deprecated constructs into errors. |
| `/Zc:strictStrings-` | `-Wno-writable-strings` | |
| (implicit) | `-fno-strict-return`, `-fno-strict-aliasing`, `-fwrapv`, `-Wno-c++11-narrowing` | Matches MSVC's code generation assumptions. |
| `WholeProgramOptimization` | ThinLTO, only with `--lto` | |
| `UNICODE`, default `.lib`s | added by the generator | MSBuild adds them implicitly. |

Windows file names are case-insensitive, and both the sources and the SDK depend on that. Under
Wine, `cl.exe` already sees a case-insensitive file system. For clang, the generator handles it as
follows:

- **Includes and resource files.** It scans for `#include`s and `.rc` resource references that only
  resolve when case is ignored. Each one gets a symlink in `<build>/case-shim/`, which is searched
  last.
- **Libraries.** `#pragma comment(lib, ...)` names get the same treatment in `<build>/lib-shim/`.

Both toolchains link the `prowizard` library built from this source tree. With Visual Studio,
`winuae` links whatever `prowizard.lib` is in `C:\dev\lib`.

## Not covered (yet)

- Only x64 is supported. The Win32 and ARM64 configurations would need the matching xwin
  architectures and libraries.
- The HLSL shaders are not compiled. The generated headers in `od-win32/shaders` are committed, so
  the build uses those.
- The generator tools (`gencpu`, `build68k`, etc.) are not run. Their outputs are committed, as they
  are for the Visual Studio build.
