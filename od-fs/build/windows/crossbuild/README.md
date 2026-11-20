# Cross-compiling FS-UAE for Windows on Linux

These scripts build the Windows version (`x86_64-w64-mingw32`) on Linux with the native Clang/LLD and MSYS2 MINGW64 packages, installed into [quasi-msys2](../quasi-msys2), for development builds and for the Windows release dist (`dist`).

## With make (in Docker)

Only Docker is needed on the host:

```sh
make -C od-fs configure-windows   # configure .build/windows (CONFIGURE_ARGS=... for extra args)
make -C od-fs build-windows -j8   # build .build/windows/fs-uae.exe (configures first if needed)
make -C od-fs clean-windows       # make clean in .build/windows
make -C od-fs dist-windows        # the release dist, in .build/release/<version> (also run by
                                  # make -C od-fs release; see FSUAE_DIST_BUILD below)
```

Each target runs `docker-run`, which builds the image from the [`Dockerfile`](Dockerfile) (Ubuntu 24.04 with Clang 20 and the other packages in [`apt-packages.txt`](apt-packages.txt); see [Ubuntu release](#ubuntu-release)), and then runs the matching script in a throwaway container as your own user. The project directory is mounted at the same path as on the host, so the build directory and quasi-msys2 work both inside and outside the container. `DOCKER=podman` selects another container runtime, and `FSUAE_WINDOWS_DOCKER_ARGS` passes extra options to `docker run`. `docker-run bash` gives a shell in the container.

## Directly on the host

To run the scripts without Docker on Ubuntu 24.04 (as the GitHub `ubuntu-24.04` runner does), set up what the image has: the packages in [`apt-packages.txt`](apt-packages.txt), and `WIN_NATIVE_CLANG_VER=20`, which makes quasi-msys2 use `clang-20`, `clang++-20`, `ld.lld-20` (as `-fuse-ld=lld-20`) and `llvm-windres-20` rather than guess which Clang to use:

```sh
sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    $(sed 's/#.*//' od-fs/build/windows/crossbuild/apt-packages.txt)
export WIN_NATIVE_CLANG_VER=20
```

`clang-20`, `lld-20` and `llvm-20` come from `noble-updates` (enabled by default). The image sets up nothing else: no other environment variables, links or alternatives, and the default `clang` (18 on Ubuntu 24.04) is neither needed nor used.

Other systems need the other tools in the list and Clang 19 or newer with LLD and `llvm-windres`: set `WIN_NATIVE_CLANG_VER` to its version suffix, or leave it unset when the unversioned `clang`, `clang++`, `ld.lld` and `llvm-windres` are new enough. (Without `WIN_NATIVE_CLANG_VER`, quasi-msys2 uses the highest-sorting `clang-<version>` if `clang++-<version>` and `lld-<version>` exist too, and otherwise the unversioned tools.) The scripts stop if a tool is missing (`check_tools` in `common.sh`) or the Clang that quasi-msys2 picked is older than 19.

Then run the scripts:

```sh
od-fs/build/windows/crossbuild/configure [configure args]
od-fs/build/windows/crossbuild/build [make args, e.g. -j8]
od-fs/build/windows/crossbuild/clean
od-fs/build/windows/crossbuild/dist [make args]   # the release dist, see Scripts below
```

Nothing else needs to be set up first: on the first run, `configure` (or `build`, or `dist`) sets up quasi-msys2 in `.build/quasi-msys2`, downloads the MSYS2 packages into it and cross-compiles libmpeg2 and PortMidi (see `quasi-msys2.sh`). This needs no root and takes a few minutes; later runs skip it.

## MSYS2 packages

The MSYS2 packages are pinned: they are installed from the snapshot in [`od-fs/build/windows/msys2`](../msys2), not from the current MSYS2 repository, so every build gets the same package versions until the snapshot is updated.

- `requested_packages.txt`: the packages to install (their dependencies are added). Clang, LLD, pkgconf, CMake, Ninja, GDB and uv are for native MSYS2 builds; quasi-msys2 installs them too, but the cross-build uses the distribution's tools.
- `database.mk`: the MSYS2 MINGW64 package database (as processed by quasi-msys2) that they are installed from.
- `packages.txt`: the resulting packages, with versions. The setup checks that exactly these are installed.
- `keyring.asc`: the MSYS2 package signing keys.

To move to newer packages (or after editing `requested_packages.txt`), run `od-fs/build/windows/update-msys2.sh` (needs Docker), which updates the snapshot and shows which versions changed, and commit the result. The next `configure` or `build` installs, upgrades or downgrades packages to match the snapshot. MSYS2 keeps old package files for a few years; the downloaded ones are kept in `.build/quasi-msys2/cache`.

Native MSYS2 builds on Windows use the same snapshot: in an "MSYS2 MINGW64" shell, `od-fs/build/prepare` installs exactly the versions in `packages.txt` with `pacman -U` and the package URLs in the MSYS2 repository (made by `od-fs/build/windows/msys2-urls.sh`; `update-msys2.sh` checks that the package files have the names it expects). The native builds (`od-fs/configure` and the dist entry script `od-fs/build/dist/dist`, which makes the Windows dist natively) use the snapshot's Clang and LLD unless `CC` is set (`od-fs/build/windows/msys2-native.sh`), and the dist entry script stops when the installed packages are not the pinned ones. libmpeg2 and PortMidi are built by the same script (`install_extra_libs` in `od-fs/build/windows/extra-libs.sh`) for both.

## Scripts

| Script | Does |
| --- | --- |
| `quasi-msys2.sh` | Sourced by `configure` and `build`. Sets up quasi-msys2 with the MSYS2 packages from the snapshot, and cross-compiles libmpeg2 and PortMidi (no longer in MSYS2; `../extra-libs.sh`, shared with native MSYS2). Does nothing when that is already done. They run it first, so it is set up on demand. |
| `configure` | Runs `od-fs/configure --host=x86_64-w64-mingw32` in the quasi-msys2 environment (and `od-fs/bootstrap` first if there is no `configure` yet). |
| `build` | Runs `make` in the build directory (configuring first if needed), then lays out `Libs/x86_64-w64-mingw32/` like a dist with `../dev_layout.py`, which native builds use too: `FS-UAE.exe`, the DLLs it needs and `FS-UAE._pth`, which gives Python the quasi-msys2 MinGW standard library as `Z:` paths for Wine. For the default build this is `od-fs/Libs/x86_64-w64-mingw32/`, and the proxy is copied to `od-fs/FS-UAE.exe`; for other build directories it is in the build directory, without the proxy. |
| `clean` | Runs `make clean` in the build directory. |
| `dist` | Builds the Windows release dist from scratch: sets up the cross-build environment and runs the dist entry script (`od-fs/build/dist/dist`) in it, which configures with `--enable-dist-layout` and builds in `<base>/build/x86_64-w64-mingw32`, makes the dist in `<base>/dist/x86_64-w64-mingw32`, puts the zip in `<base>/release/<version>/` and runs the dist check on it. `<base>` is `FSUAE_DIST_BUILD` (default: `.build`). The dist's Python code is byte-compiled with a native Python of the same version as the MSYS2 one, built on first use in `<quasi-msys2>/build-python-<version>`. `dist --setup-only` only sets up quasi-msys2 and that Python, for example to cache them before building (as the dist workflow does). |
| `docker-run` | Runs one of the scripts (or another command) in the Docker container. |

Environment variables:

- `WINDOWS_BUILD`: the build directory: a name, `.build/<name>` in the project directory (default: `windows`), or a path (with a `/` in it), relative to the project directory when relative. Only the default `windows` build lays out `od-fs/Libs` and `od-fs/FS-UAE.exe`.
- `QUASI_MSYS2_DIR`: where quasi-msys2 is set up (default: `.build/quasi-msys2`).
- `FSUAE_DIST_BUILD`: base directory for the `dist` script's build, dist and release directories (default: `.build`; a relative path is relative to the project directory). See `od-fs/build/dist/common.sh`.
- `WIN_NATIVE_CLANG_VER`: the version suffix of the Clang tools quasi-msys2 uses (`20` in the image; see [Directly on the host](#directly-on-the-host)). Read by quasi-msys2's `env/vars.src`.
- `MSYS2_MIRROR_URL`: MSYS2 package mirror (default: `https://mirror.msys2.org`, which redirects to a nearby mirror; `https://repo.msys2.org` is the primary repository).

They can also be passed to make, e.g. for a separate debug build:

```sh
make -C od-fs configure-windows WINDOWS_BUILD=windows-debug CONFIGURE_ARGS="CFLAGS='-g -O0'"
make -C od-fs build-windows WINDOWS_BUILD=windows-debug
```

Run the result with `wine FS-UAE.exe` from `od-fs`, or for another build directory, `wine Libs/x86_64-w64-mingw32/FS-UAE.exe` from there. The MinGW `bin` folder does not have to be in Wine's `PATH`: the layout is the same as for a native build (see "The layout" in [Building.md](../../../Docs/Development/Building.md#the-layout)). FS-UAE finds its Python code and resources two levels above `Libs/x86_64-w64-mingw32/FS-UAE.exe`: in `od-fs`, or in the build directory's `Python` and `Resources`, links to `od-fs` that `od-fs/configure` makes (Wine follows them).

## Ubuntu release

The image is Ubuntu 24.04, like the Linux dist image ([`od-fs/build/linux`](../../linux)), with Clang, LLD and LLVM 20 (`clang-20`, `lld-20` and `llvm-20` from `noble-updates`), so that a GitHub `ubuntu-24.04` runner can run the scripts directly with the same packages, which the Windows job of the dist workflow (`.github/workflows/dist.yml`) does.

Ubuntu 24.04's default Clang 18 cannot be used: it cannot compile the C++ standard library headers of the MSYS2 GCC (16.2 in the snapshot), which Clang uses when cross-compiling. Code using `std::string` or `std::vector` iterators fails, for example:

```text
.../root/mingw64/include/c++/16.2.0/bits/stl_iterator.h:1337:19: error: member access into
incomplete type 'const __normal_iterator<const char *, basic_string<char>>'
```

Clang 19 and newer compile them. A later snapshot with a newer GCC may need a newer Clang again: then change the version in `apt-packages.txt` and `WIN_NATIVE_CLANG_VER` in the `Dockerfile` (and this README), and `CLANG_MIN_VERSION` in `common.sh` if needed.

The snapshot's own Clang (23 in `mingw-w64-x86_64-clang`, for native MSYS2 builds) is not used by the cross-build, and its version does not have to match: quasi-msys2 only requires that for the `CLANG*` MSYS2 environments, not for MINGW64.

## Notes

- The scripts source only `env/vars.src` from quasi-msys2, never `env/all.src`: `all.src` registers Wine for `.exe` files via sudo, and if Wine is installed it puts Wine wrappers for the MSYS2 tools (`python`, `gcc`, ...) first in `PATH`, so configure ends up running Windows programs in Wine.
- There is no `/mingw64` symlink (creating it would need root). quasi-msys2 uses absolute paths instead, so the build and quasi-msys2 directories must stay at the same path; this is why `docker-run` mounts the project at its host path.
