# Cross-compiling FS-UAE for Windows on Linux

These scripts build the Windows version (`x86_64-w64-mingw32`) on Linux with the native Clang/LLD
and MSYS2 MINGW64 packages, installed into [quasi-msys2](../../../dist/docker/quasi-msys2). It is
the same approach as the Docker release build in `od-fs/dist/docker`, but for development builds.

## With make (in Docker)

Only Docker is needed on the host:

```sh
make -C od-fs configure-windows   # configure .build/windows (CONFIGURE_ARGS=... for extra args)
make -C od-fs build-windows -j8   # build .build/windows/fs-uae.exe (configures first if needed)
make -C od-fs clean-windows       # make clean in .build/windows
```

Each target runs `docker-run`, which builds the image from the [`Dockerfile`](Dockerfile) (an
Ubuntu 26.04 image with the build tools), and then runs the matching script in a throwaway
container as your own user. The project directory is mounted at the same path as on the host, so
the build directory and quasi-msys2 work both inside and outside the container. `DOCKER=podman`
selects another container runtime, and `FSUAE_WINDOWS_DOCKER_ARGS` passes extra options to
`docker run`. `docker-run bash` gives a shell in the container.

## Directly on the host

Install the same Ubuntu packages as the [`Dockerfile`](Dockerfile):

```sh
sudo apt-get install --no-install-recommends \
    ca-certificates curl wget git build-essential \
    make tar xz-utils zstd gawk gpg gpgv \
    clang lld llvm binutils-mingw-w64-x86-64 \
    autoconf automake libtool cmake ninja-build \
    gettext pkgconf python3 zip unzip file
```

Then run the scripts:

```sh
od-fs/build/windows/crossbuild/configure [configure args]
od-fs/build/windows/crossbuild/build [make args, e.g. -j8]
od-fs/build/windows/crossbuild/clean
```

Nothing else needs to be set up first: on the first run, `configure` (or `build`) sets up
quasi-msys2 in `.build/quasi-msys2`, downloads the MSYS2 packages into it and cross-compiles
libmpeg2 and PortMidi (see `quasi-msys2.sh`). This needs no root and takes a few minutes; later runs
skip it. If a tool is missing, the scripts stop and list the missing commands.

## Scripts

| Script | Does |
| --- | --- |
| `quasi-msys2.sh` | Sourced by `configure` and `build`. Sets up quasi-msys2 with the MSYS2 packages, and cross-compiles libmpeg2 and PortMidi (no longer in MSYS2). Does nothing when that is already done. They run it first, so it is set up on demand. |
| `configure` | Runs `od-fs/configure --host=x86_64-w64-mingw32` in the quasi-msys2 environment (and `od-fs/bootstrap` first if there is no `configure` yet). |
| `build` | Runs `make` in the build directory (configuring first if needed), then lays out `Libs/x86_64-w64-mingw32/` like a dist: `FS-UAE.exe`, the DLLs it needs and a `lib` link to the MinGW standard library. For the default build this is `od-fs/Libs/x86_64-w64-mingw32/`, and the proxy is copied to `od-fs/FS-UAE.exe`; for other build names it is in the build directory. |
| `clean` | Runs `make clean` in the build directory. |
| `docker-run` | Runs one of the scripts (or another command) in the Docker container. |

Environment variables:

- `WINDOWS_BUILD`: build directory name, `.build/<name>` in the project directory (default:
  `windows`).
- `QUASI_MSYS2_DIR`: where quasi-msys2 is set up (default: `.build/quasi-msys2`).
- `MSYS2_MIRROR_URL`: MSYS2 package mirror (default: `https://mirror.msys2.org`, which redirects
  to a nearby mirror; `https://repo.msys2.org` is the primary repository).

They can also be passed to make, e.g. for a separate debug build:

```sh
make -C od-fs configure-windows WINDOWS_BUILD=windows-debug CONFIGURE_ARGS="CFLAGS='-g -O0'"
make -C od-fs build-windows WINDOWS_BUILD=windows-debug
```

Run the result with `wine FS-UAE.exe` from `od-fs`.

## Notes

- The scripts source only `env/vars.src` from quasi-msys2, never `env/all.src`: `all.src` registers
  Wine for `.exe` files via sudo, and if Wine is installed it puts Wine wrappers for the MSYS2 tools
  (`python`, `gcc`, ...) first in `PATH`, so configure ends up running Windows programs in Wine.
- There is no `/mingw64` symlink (creating it would need root). quasi-msys2 uses absolute paths
  instead, so the build and quasi-msys2 directories must stay at the same path; this is why
  `docker-run` mounts the project at its host path.
