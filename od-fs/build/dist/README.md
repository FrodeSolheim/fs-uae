# Dist scripts

The scripts that make FS-UAE's dists (the portable Linux, Windows and macOS downloads), check them, and publish them on GitHub, and the packaging for other kinds of packages. The overview is in [Distributions.md](../../Docs/Development/Distributions.md); the versions and origins the scripts give the dists are described in [Versions.md](../../Docs/Development/Versions.md). (`README.txt` here is not about the scripts: it is the README that every dist has.)

## Usage

```sh
od-fs/build/dist/dist [<triplet> [<make argument>...]]
```

makes a complete dist for the triplet (default: the host's) in the current environment: it configures FS-UAE for the dist layout and builds it, assembles the dist, writes its archive and runs the dist check on it. It does not set up an environment: the make targets (`make -C od-fs dist-linux`, `dist-windows`, `dist-macos` and the others, see `od-fs/Makefile`) run it in the environment for each platform, and the dist workflow (`.github/workflows/dist.yml`) runs it on GitHub's runners.

The output goes under a base directory, `FSUAE_DIST_BUILD` (default `.build` in the project directory; a relative path is relative to the project directory):

| Directory | Holds |
| --- | --- |
| `<base>/build/<triplet>` | The build directory (configured from scratch every time) |
| `<base>/dist/<triplet>` | The dist |
| `<base>/release/<version>` | The archive: `FS-UAE-<version>-<triplet>.tar.xz`, `.zip` or `.dmg` |
| `<base>/deb` | The Debian source package (`obs/deb-src.sh`) |

## Files

| File | Does |
| --- | --- |
| `dist` | The entry script (above). |
| `common.sh` | Settings shared by the scripts (sourced): the output directories, the build environment for the build header, and macOS signing. |
| `version.py` | Prints the version a dist is made for, from `AC_INIT` in `configure.ac` and git; with `--commit`, the commit. |
| `build.sh` | Configures (with `--enable-dist-layout` and the version and identity) and builds FS-UAE in the build directory. |
| `assemble.sh` | Assembles the dist from the build (the program, its libraries, the Python home, the resources, `README.txt`, `Extras/Portable.ini`) and writes the archive; on macOS, signs and notarizes it when it can. |
| `linux_libs.py` | Finds and copies the libraries a Linux dist bundles, by walking the dependencies of the executable and Python's extension modules; it fails on a library that is neither found nor on its list of libraries that come from the user's system. |
| `check.py` | The dist check: `check.py <archive or dist directory> <triplet>` checks a dist without running it (the archive name and version, the layout, the README's line endings, Python, the libraries or DLLs, the build header; for macOS the minimum versions, `Info.plist`, the signatures and entitlements, and what is on the `.dmg`, which it opens on a Mac). |
| `macho.py` | Reads Mach-O files for the dist check's macOS checks, without macOS tools. |
| `dmg.py` | Mounts a `.dmg` read-only with `hdiutil` (on a Mac), for the dist check and the smoke test. |
| `smoke.py` | The dist smoke test: `smoke.py <archive or dist directory> <triplet>` starts a Linux or macOS dist (a macOS one from its `.dmg`) with the built-in AROS ROM and checks that it runs (see the script). Run by hand, and by the dist workflow for Linux x86_64 and the macOS dists. |
| `png_reader.py` | Reads the PNG files of FS-UAE's GL grab, for the smoke test here, the WHDLoad smoke test and the compositor test (`od-fs/tests`). |
| `release_files.py` | Gathers the files of a GitHub release from a workflow run's artifacts and writes `SHA256SUMS`; with `--dev`, those of the `dev` pre-release. |
| `draft-release` | Makes the draft GitHub Release for a tag with `gh`, or replaces the files of an existing draft. |
| `dev_release.py` | Replaces the files of the rolling `dev` pre-release with `gh`. |
| `README.txt` | The README of every dist. |
| `macos/` | The macOS entitlements, the `.dmg` settings (`settings.py`, for dmgbuild) and `signing-keychain`, which puts the signing certificate from the workflow's secrets into a temporary keychain. |
| `obs/` | The OBS packaging: the Debian files, the RPM spec, `deb-src.sh` (the Debian source package), `obs.sh` (copies it into the OBS package) and `obs-origin.sh` (the packages' origin). |
| `flatpak/` | The Flatpak manifest, stale and not built. |

## Environment variables

| Variable | Default | Meaning |
| --- | --- | --- |
| `FSUAE_DIST_BUILD` | `.build` | The base directory for the output (above). |
| `FSUAE_ENV_DIR` | `.build/env` | Where the env prefixes are (Linux and macOS; see `od-fs/build/env/scripts/paths.sh`). |
| `FSUAE_DIST_ORIGIN` | `Unofficial build` | Who makes the dist, for the build header. |
| `FSUAE_DIST_ENVIRONMENT` | The operating system, and the Docker or runner image | What the dist is made in, for the build header. |
| `FSUAE_MACOS_SIGNING` | `auto` | Whether macOS dists are signed and notarized: `auto` (when the credentials are there), `required` or `off`. |
| `NOTARYTOOL_APPLE_ID`, `NOTARYTOOL_PASSWORD`, `NOTARYTOOL_TEAM_ID` | - | The notarization credentials (with the Developer ID certificate in a keychain). |
| `FSUAE_MACOS_KEYCHAIN` | the keychain search list | The keychain with the signing certificate (set by `macos/signing-keychain`). |

The Docker wrappers (`od-fs/build/linux/docker-run`, `od-fs/build/windows/crossbuild/docker-run`) pass `FSUAE_DIST_BUILD`, `FSUAE_DIST_ORIGIN` and `FSUAE_DIST_ENVIRONMENT` on to the container, and the Linux one `FSUAE_ENV_DIR` too.
