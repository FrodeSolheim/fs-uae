## quasi-msys2

A Linux-to-Windows cross-compilation environment. Imitates [MSYS2](https://www.msys2.org/) (which is Windows-only) on Linux.

Features:

* [Huge amount of prebuilt libraries](https://packages.msys2.org/package/), and [several MinGW flavors](https://www.msys2.org/docs/environments/) (all of this comes from the MSYS2 project).
* Linux-distribution-agnostic.
* The installation is self-contained.

Here's how it works:

* **Libraries:** Prebuilt libraries are downloaded from MSYS2 repos (the standard library and any third-party libraries you need).

* **Compiler:** The recommended choice is Clang (any native installation works, you don't need a separate version targeting Windows), quasi-msys2 makes it cross-compile by passing the right flags to it.<br/>
  Alternatively, you can [install MinGW GCC](#how-do-i-use-a-different-compiler) from your distro's package manager and use that.<br/>
  Alternatively, quasi-msys2 can download MSYS2 GCC/Clang and run them in Wine, but this is not recommended (slow and the build systems sometimes choke on it).

* **Build systems:** Must be installed natively. We make them cross-compile by passing the right flags and config files.

* **Cygwin-based MSYS2 packages:** Are not available (because Cygwin doesn't work well under Wine, if at all), but they aren't very useful, because the same utilities are available on Linux natively.

* **Package manager:** MSYS2 `pacman` also uses Cygwin, so we replace it with a small custom package manager.

* **Wine:** Optionally, [`binfmt_misc`](https://en.wikipedia.org/wiki/Binfmt_misc) allows Windows executables to be transparently invoked via Wine. (This can help if your build system tries to run cross-compiled executables during build, and doesn't provide a customization mechanism to explicitly run Wine.)

## Usage

* Install dependencies:

  * **Ubuntu / Debian:** `sudo apt install make wget tar zstd gawk gpg gpgv wine`

    * Install latest LLVM, Clang and LLD using `bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"`. Or you can try the stock ones from Ubuntu repos, but they are often outdated.

  * **Arch:** `sudo pacman -S --needed make wget tar zstd gawk gnupg wine llvm clang lld`

    * To install Wine, you need to [enable the `multilib` repository](https://wiki.archlinux.org/title/official_repositories#Enabling_multilib) first.

    * Clang in the repos is usually outdated by one major version. If you don't like that, build from source or use AUR.

  * **Fedora:** `sudo dnf install make wget tar zstd gawk which gpg wine llvm clang lld`

  * **Termux:** `sudo pkg install make wget tar zstd gawk which gnupg gpgv llvm clang lld`

    * In Termux all package manager operations below (`make install ...`) have to be peformed as `proot --link2symlink make install ...` (otherwise we can't extract package archives with hardlinks in them). The package installation will take a long time.

  * (similarly for other distros)

  Wine is optional but recommended. `make --version` must be 4.3 or newer. Clang is the recommended compiler choice, but if you [use something else](#how-do-i-use-a-different-compiler), you don't have to install it.

* Install quasi-msys2:
  ```bash
  git clone https://github.com/holyblackcat/quasi-msys2
  cd quasi-msys2
  make install _gcc _gdb # same as `make install mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-gdb`
  ```
  You can also [`make install` third-party libraries](#package-manager-usage), if MSYS2 provides them. You can skip `_gdb` if you don't need a debugger.

  For selecting the MSYS2 environment (the flavor of MinGW), [see FAQ](#how-do-i-use-different-msys2-environments).
* Open quasi-msys2 shell:
  ```bash
  env/shell.sh
  ```
  This adds MSYS2 packages to `PATH`, and sets some environment variables. For non-interactive use, see [this](#how-do-i-run-commands-non-interactively).

* Build:
  * Manually:
    ```bash
    win-clang++ 1.cpp # Calls your Clang with the right flags for cross-compilation.
    ./a.exe # Works if you installed Wine.
    ```
    You can also use `g++` and `clang++` to run the respective MSYS2 compilers in Wine, assuming you installed `_gcc` and `_clang` respectively.
  * With Autotools: `./configure && make` as usual, no extra configuration is needed.
  * With CMake: `cmake` as usual.
  * With Meson: `meson` as usual.

* Other tools that work in `env/shell.sh`:
  * `gdb` and `lldb` (assuming the respective MSYS2 packages are installed). If the keyboard input doesn't work, try `wineconsole gdb ...`.
  * `pkg-config` and `pkgconf` (the native ones must be installed, don't need the MSYS2 packages).
  * `win-ldd`, which lists the `.dll`s an executable depends on  (must install `ntldd` MSYS2 package, `win-ldd` wraps `wine ntldd -R` and convers the paths to Linux style).
  * `windres` (runs whatever Windres it can find, either the LLVM one, the native MinGW one, or the MSYS2 one through Wine)

* Accessing non-cross compilers and other native tools:

  * Use absolute paths (e.g. `/usr/bin/gcc`) to access non-cross compilers and tools (CMake, Meson, etc), if you need to produce a Linux executable.

  * The only exception is `win-native-pkg-config` to access the native `pkg-config`, because we control pkg-config using environment variables rather than by providing a custom executable. (The `win-native-pkg-config` helper script simply unsets all pkg-config-related environment variables before running it.)

### Rust

I try to support Rust for completeness, but the support is experimental.

You don't need any extra MSYS2 packages (other than `make install _gcc` for the libraries). Install `rustup` natively (outside of quasi-msys2) and run `rustup target add $CARGO_BUILD_TARGET` inside `env/shell.sh` to install the standard library for the target platform (calling it inside solely because `env/shell.sh` sets `CARGO_BUILD_TARGET`, you can call it from anywhere if you know the value).

Then you can use:

* `cargo` (we set environment variables to make it cross-compile by default).

* `win-rustc` to compile a single file (this wrapper calls `/usr/bin/rustc` with flags for cross-compilation).<br/>
  Use `host-rustc` to compile for the host system (like `rustc` outside of quasi-msys2 shell).<br/>
  The plain `rustc` will likely not work correctly in quasi-msys2 shell.

## Package manager usage

Run `make help` to get the full list of commands it supports.

Here are some common ones:

* `make list-all` - List all packages available in the repository.<br>
  This command will only download the repository database on the first run. Updating the database is explained below.

  Use `make list-all | grep <package>` to search for packages.

* `make install <packages>` - Install packages.<br>
  The packages are installed to the `./root/`.

  <sup>Most package names share a common prefix: `mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-clang ...`. You can use `_` instead of this long prefix, e.g. `make install _gcc` instead of `make install mingw-w64-ucrt-x86_64-gcc`.</sup>

* `make remove <packages>` - Remove packages.

* `make upgrade` - Download the latest package database and install package updates.<br>
  Do this routinely to keep your installation up to date.

  The last update can be rolled back using `make rollback`.

* `make list-ins` - List all installed packages.

* `make list-req` - List only those installed packages that were explicitly requested, rather than being automatically installed as a dependency.

* `make apply-delta` - Resume interrupted package installation/removal.

* `make reinstall-all` - Reinstall all packages, if you screwed up your installation.

* Previewing changes before applying them:

  Normally the packages are installed immediately without asking. If you want to check what will be installed first, you can do following:

  * Instead of `make upgrade`, do `make update` and `make delta` to list the changes. Then `make apply-delta` to apply.

  * Instead of `make install ...`, do `make request ...` and `make delta` to list the changes. Then `make apply-delta` to apply or `make undo-request ...` to back out.

  * Instead of `make remove ...`, do `make undo-request ...` and `make delta` to list the changes. Then `make apply-delta` to apply or `make request ...` to back out.

**Known issues**

* Pre/post-install actions are not executed; we simply unpack the package archives. In most cases this is good enough.

* If a package depends on a specific version of some other package, the exact version of that package is not checked. This shouldn't affect you, as long as you don't manually install outdated packages.

* Package conflicts are handled crudely. We don't respect the conflict annotations in the packages, but at least we refuse to overwrite files, which should normally be enough.

* You can't run several instances of the package manager in the same installation in parallel. There's no locking mechanism, so this can cause weird errors.

## Backing up the installation

The whole installation directory can be moved around, it doesn't contain any absolute paths.

But you don't need to copy everything if you're making a backup, assuming all files came from the package manager. You only need a clean copy of the repository, and following files:

* `database.mk` — The package database.
* `requested_packages.txt` — The list of packages you've explicitly installed.
* Contents of the `cache/` directory, which contains archived versions of all installed packages. Before backing up the cache, make sure it's up-to-date and minimal by running `make cache-installed-only`.
* User config files: `msystem.txt`, `alternatives.txt` (if present).

To restore such backup to a working state, run `make apply-delta` in it.

Outdated packages linger in the repos for a few years, so if you just want to lock specific package versions, you don't need to backup the `cache`.

## FAQ

### How do I run commands non-interactively?

`env/shell.sh` works best for interactive use.

If you want to run commands non-interactively (as in from shell scripts), do this:

```sh
bash -c 'source env/all.src && my_command'
```

If you don't want certain components of the environment, you can study `all.src` and run desired components manually. (E.g. if you don't want `binfmt_misc`.)

### How do I use different [MSYS2 environments](https://www.msys2.org/docs/environments/)?

The environment can be changed using `echo ... >msystem.txt` (where `...` can be e.g. `MINGW64`), preferably in a clean repository. If you want multiple environments, you need multiple copies of quasi-msys2.

All environments should work, more or less. (Except for `MSYS`, which I'm not particulary interested in, since Cygwin doesn't seem to work with Wine. Also `CLANGARM64` wasn't tested at all.)

On `CLANG64`, when using the native Clang, you must install the same native Clang version as the one used by MSYS2 (only the major version must match).

#### How to choose the environment?

`UCRT64` is a good default. Use `MINGW64` if you want the old C standard library (`msvcrt.dll` instead of `ucrtbase.dll`).

I don't see a good reason to use `CLANG64` (other than the ability to build with sanitizers, but the resulting executable won't run in Wine anyway), and it has a downside of locking you to a specific native Clang version.

### How do I use a different compiler?

You can study [`env/vars.src`](/env/vars.src) for the environment variables you can customize.

We support the following compilers. By default we pick the first one that works (and set `CC`,`CXX` to point to it), but you can override the choice by setting `WIN_DEFAULT_COMPILER` env variable to the respective compiler name.

* **Native Clang** (`native_clang`)

  The recommended option. Requires Clang to be installed on the system (the regular version of Clang, nothing MinGW-specific). See the beginning of this file for the recommended installation strategy.

  We provide `win-clang`, `win-clang++` scripts that will call your native Clang with the correct flags for cross-compilation.

  You have to install a compiler in quasi-msys2 for this to work, just to provide the basic libraries. `make install _gcc` in most [environments](#how-do-i-use-different-msys2-environments), or `make install _clang` in `CLANG64` environment.

  We're using the LLD linker by default, so that should be installed too, but in theory you can configure Clang to use something else.

  We need the `llvm` package (as opposed to Clang and LLD) solely for `llvm-windres`. If you don't need Windres, you can skip it.

  If using the `CLANG64` [environment](#how-do-i-use-different-msys2-environments), the major version of the native Clang you have must match the version of MSYS2 Clang you installed in quasi-msys2. And remember that like in MSYS2, in quasi-msys2 there is no simple way to install an outdated package unless you backed up `database.mk` package database from before the update; so it's easier to change the system Clang version to match, this is easiest to do on Ubuntu/Debian since https://apt.llvm.org/ lets you freely choose the version.

  * You can set `WIN_NATIVE_CLANG_FLAGS` to customize what flags are passed to your native Clang. We print the guessed flags when initializing `env/shell.sh`.

  * You can set `WIN_NATIVE_CLANG_VER` to a single number (e.g. `19`) if your native Clang is suffixed with a version (e.g. `clang++-19`), or `NONE` if not suffixed (just `clang++`). We try to guess this number. You can also specify custom native Clang binaries with `WIN_NATIVE_CLANG_{CC,CXX,LD}`.

* **Native MinGW GCC** (`native_gcc`)

  This is a version of MinGW GCC installed from your system. This is not usable on the `CLANG64` environment. The specific package you need to install depends on the environment, see the table below.

  We provide `win-gcc`, `win-g++` scripts that will call your native GCC with the adjusted header and library search paths.

  **NOTE:** The behavior depends on whether you also install GCC in MSYS2 or not. It's better not to by default, if you want to use this compiler. If it's installed, we'll use the standard library from MSYS2 GCC instead of the one from your native GCC (we're forced to, because otherwise both will be in the search path and will conflict). This sounds a bit sketchy, especially so if the GCC versions don't match (in theory, judging by the directory names, the full X.Y.Z version number must match, but it remains to be seen how important this is).

  Which package to install:

  &nbsp;|MINGW32|MINGW64|UCRT64|Comments
  ---|---|---|---|---
  **Ubuntu / Debian**|`g++-mingw-w64-i686-posix`|`g++-mingw-w64-x86-64-posix`|`g++-mingw-w64-ucrt64`|<sup>1. The UCRT64 packages were added recently and might not exist on older LTS distro versions.<br/>2. There are also packages suffixed with `-win32` instead of `-posix`, which use a different "thread model". Quasi-msys2 will refuse to use them. I didn't test if they'd work or not, but it sounds like a bad idea, since MSYS2 uses the "posix" mode, and so do the mingw packages in all other distros. The UCRT64 package always uses the "posix" mode.<br/>3. There are also `gcc-...` packages that only include the C compiler and not the C++ one.</sup>
  **Arch**|N/A|`mingw-w64-gcc`|N/A
  **Fedora**|`mingw32-gcc-c++`|`mingw64-gcc-c++`|`ucrt64-gcc-c++`|<sup>There are also package without the `...-c++` suffix that only include the C compiler and not the C++ one.</sup>

  Some customizations:

  * We try to guess the compiler executable name, but you can override the detection by setting the `WIN_NATIVE_GCC_{CC,CXX}`, env variables.

  * You can also override the compiler flags using `WIN_NATIVE_GCC_FLAGS`. Consult the default value which is logged during intialization.

* **MSYS2 Clang** (`msys2_clang`)

  This will run in Wine. Fine for a hello world, but build systems tend to choke on this.

  Obviously the compiler needs to be installed in quasi-msys2 for this to work.

* **MSYS2 GCC** (`msys2_gcc`)

  This will run in Wine. Fine for a hello world, but build systems tend to choke on this.

  Obviously the compiler needs to be installed in quasi-msys2 for this to work.

  This is not available in the `CLANG64` environment.

Some other customizations are:

* Using an entirely custom cross-compiler:

  * You can set `WIN_CC` and `WIN_CXX` to any compiler. This overrides the `WIN_DEFAULT_COMPILER=...` and the default compiler detection. `env/shell.sh` will set `CC`, `CXX` to the values of those variables.

* Customizing the native compiler that's used for non-cross compilation. This is something we only report to the build systems (currently only Meson), and don't use directly.

  * The specified `CC`, `CXX`, `LD` will be used for this. Their values are then replaced with the cross-compiler by `env/shell.sh`.

  * To override `CC`, `CXX` set by `env/shell.sh` (which will be used for cross-compiling), set `WIN_CC` and `WIN_CXX` respectively.


### How do I add a desktop entry for the quasi-msys2 shell?

There's a tiny script to install a shortcut. Right now there are no different shortcuts for different MSYS2 environments.

Use `make -f env/integration.mk` to install. To undo, invoke it again with the `uninstall` flag.

### Using LD instead of LLD when compiling with the native Clang.

I started having problems with the native LD after some MSYS2 update (it produces broken executables), so we default to LLD.

Last tested on LD 2.34, a more recent version might work.

LD shipped by MSYS2 (was LD 2.37 last time I checked) works under Wine. If `binfmt_misc` is enabled, you can switch to it using `-fuse-ld=$MSYSTEM_PREFIX/bin/ld.exe`.

You can try the native LD using `-fuse-ld=ld`.

### My build system is confused because the compiled C/C++ binaries are suffixed with `.exe`.

Use `source env/duplicate_exe_outputs.src`. Then `$CC` and `$CXX` will output two identical binaries, `foo.exe` and `foo`. The lack of the extension doesn't stop them from being transparently invoked with Wine.


## Installation structure

* `Makefile` — The package manager.

* `root/` — Packages are installed here.

* `index/` — For each installed package it contains a file with a list of files owned by it.

  `root/` and `index/` should always stay in sync, otherwise things can break. But you can install your own files to `root/`.

* `cache/` — Stores cached archives of the packages downloaded from the repo.

  Also stores archive signatures. They're checked at download time, and are preserved for informational purposes only.

* `database.mk` — The package database, converted to our own format.

* `database.mk.bak` — A backup of `database.mk` performed the last time a new database was downloaded.

* `database.current_original[.sig]` — The original database file downloaded from the repository. This is used to speed up database updates (if the downloaded database matches this file, we don't need to reparse it).

   The signature is checked at download time, and is preserved for informational purposes only.

* `requested_packages.txt` — A list of installed packages, not including the automatically installed dependencies.

* `alternatives.txt` — Exists only if you created it manually. A configuration file for package alternatives, see `make help` for details.

* `msystem.txt` — Exists only if you created it manually. Configures MSYS2 flavor, see `make help` for details.

* `msys2_pacmake_base_dir` — An empty file marking the installation directory. The package manager refuses to operate if it's not in the working directory, to make sure you don't accidentally create a new installation.

* (temporary) `database.db{,.unverified,.sig}` — The database downloaded from the repository, in the process of being converted to our custom format (`.unverified` is before the signature check).

* (temporary) `database/` — Temporary files created when processing a downloaded database.

* `env/` — Contains the scripts for configuring the build environment. The contents have no connection with the package manager.

  * `binfmt.mk` — Configures the kernel to transparently run Wine programs. It uses `sudo`, so you'll be asked for a `sudo` password.

    Has flags to un-configure the kernel, run it to get more information.

  * `fakebin.mk` — Generates extension-less wrappers for all installed executables, to make running them easier.

    Has a flag to delete all wrappers, run it to get more information.

  * `fake_bin/` — Contains the wrappers generated by `fakebin.mk`

  * `vars.src` — Sets up environment variables, including `PATH`. Must be run as `source path/to/vars.src`.

  * `generate_meson_config.mk` — Generates `meson_cross_file.ini` and `meson_native_file.ini`. I couldn't figure out how to read environment variables in them, if possible at all, so they are generated.

  * `all.src` — Runs all the files above, in quiet mode. Must be run as `source path/to/all.src`.

  * `shell.sh` — Creates a new Bash shell and runs `source all.src` in it. Do `exit` to return to the original shell.

  * `integration.mk` — Generates a desktop file for the quasi-msys2 shell.

  * `duplicate_exe_outputs.src` — Modifies `CC` and `CXX` variables to point to wrappers that duplicate the produced executables without extensions. This can have with some build systems.

  * `wrappers/` — Wrappers for the native Clang and CMake that add the correct parameters for them.

  * `config/` — Contains configuration files for the build systems.

    * `config.site` — This configures the Autotools. `vars.src` stores a path to it in `CONFIG_SITE`, which Autotools read.

    * `toolchain.cmake` — This configures CMake. Our CMake wrapper passes this file to CMake.

  * `internal/` — Internal helper scripts.
