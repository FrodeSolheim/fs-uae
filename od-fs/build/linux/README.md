# Linux dist environment

The environment the Linux dists are made in: Ubuntu 24.04 (glibc 2.39), so that the dists run on every Linux distribution with glibc 2.39 or newer, whatever the host runs. Locally it is a Docker image; on GitHub, the dist workflow installs the same packages on Ubuntu 24.04 runners. See [Environment.md](../../Docs/Development/Environment.md) for the build environments and [Distributions.md](../../Docs/Development/Distributions.md) for the dists.

## Usage

```sh
make -C od-fs dist-linux           # the dist for the host's architecture
make -C od-fs dist-linux-aarch64   # aarch64, cross-compiled on an x86_64 host
make -C od-fs dist-linux-x86_64    # x86_64, cross-compiled on an aarch64 host
od-fs/build/linux/docker-run bash  # a shell in the container
```

The make targets run the dist entry script (`od-fs/build/dist/dist <triplet>`) with `docker-run`. The first run builds the image, and the env prefix with SDL3 and Python (`.build/env/<triplet>`); later runs reuse both. A cross-compiled dist also sets up the host's own prefix first, whose Python is the build Python.

## Files

| File | Does |
| --- | --- |
| `Dockerfile` | The image: `ubuntu:24.04` with the packages in `apt-packages.txt`, for both architectures (multi-arch): the build tools for the image's own architecture, the development packages for x86_64 and aarch64, and the cross compiler for the other architecture. It holds only Ubuntu packages: SDL3, SDL3_image, SDL3_ttf and Python are built by the env system at dist time, into the mounted project. |
| `docker-run` | Runs a command in a throwaway container made from the image (built first, quick when cached): as your own user, with `HOME=/tmp`, the project directory mounted at the same path as on the host (the env prefixes and build directories have absolute paths in them), and a TTY only when run from a terminal. |
| `apt-packages.txt` | The Ubuntu packages: `[tools]` (for the build machine) and `[libraries]` (development packages for the dist's architecture, as plain names). There are no development packages for the Python modules the dists leave out. |
| `apt-packages` | Prints a section of `apt-packages.txt`: `apt-packages tools` or `apt-packages libraries`. The Dockerfile and the dist workflow use it; the workflow appends the runner's architecture to the libraries (`libflac-dev:arm64`). |
| `x86_64-linux-gnu.cmake`, `aarch64-linux-gnu.cmake` | CMake toolchain files for cross-compiling to each architecture with Ubuntu's cross compiler, for the env system's cross mode (`od-fs/build/env/setup --host <triplet>`). |
| `config.site` | Answers for configure checks that cannot run when cross-compiling, for the env recipes and FS-UAE's configure in cross mode. |

## Environment variables

| Variable | Default | Meaning |
| --- | --- | --- |
| `DOCKER` | `docker` | The container command (for example `podman`). |
| `FSUAE_LINUX_DOCKER_IMAGE` | `fs-uae-linux-build` | The image's tag. |
| `FSUAE_LINUX_DOCKER_ARGS` | - | Extra arguments for `docker run`. |
| `FSUAE_DIST_BUILD`, `FSUAE_ENV_DIR` | `.build`, `.build/env` | Passed on, and mounted when outside the project (see `od-fs/build/dist/common.sh` and `od-fs/build/env/scripts/paths.sh`). |
| `FSUAE_DIST_ORIGIN`, `FSUAE_DIST_ENVIRONMENT` | - | Passed on (see `od-fs/build/dist/common.sh`). |

## Without Docker

On Ubuntu 24.04 (as on the GitHub runners), the dist entry script (`od-fs/build/dist/dist`) also runs directly, once the packages in `apt-packages.txt` are installed for the host's architecture: the Linux job of the dist workflow (`.github/workflows/dist.yml`) shows how. Cross-compiling needs the other architecture's development packages and cross compiler too, as the image has them. On other distributions, use Docker.
