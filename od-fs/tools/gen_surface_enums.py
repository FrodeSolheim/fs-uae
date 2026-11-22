#!/usr/bin/env python3
"""Generates the enums of the surface API's version 1, od-fs/Python/fsapp/surfaceapi/_enums_v1.py.

  od-fs/tools/gen_surface_enums.py [--include-dir DIR] [--output FILE]

Scancode, Keycode, MouseButton (IntEnum) and Keymod (IntFlag) come from SDL3's headers
(SDL_scancode.h, SDL_keycode.h and SDL_mouse.h), with SDL's names minus the prefix, except DIGIT_0
to DIGIT_9 for the digit keys; Navigation comes from fsapp's navigation values
(od-fs/fsapp/fsapp-navigation.h).

The headers are those of the SDL version FS-UAE is built with, the env recipe's
(od-fs/build/env/packages/sdl3), so that the enums do not depend on the SDL version installed:
--include-dir names a directory with SDL3/ in it (used whatever its version, for regenerating after
the recipe's version changed), else the first of these that has that version is used: the env
prefixes (<FSUAE_ENV_DIR>/<triplet>/include, FSUAE_ENV_DIR by default .build/env, relative to the
project directory), pkg-config's sdl3, /usr/include, /usr/local/include and /opt/homebrew/include.
od-fs/tests/fsapp/test_surface_enums.py checks that the module is up to date.

The module is version 1's: a change to it is a change to version 1, which od-fs/tests/fsapp/
test_surface_api.py catches, and a later version of the surface API gets a module of its own.
"""

import argparse
import keyword
import os
import re
import subprocess
import sys
from pathlib import Path

REPO_DIR = Path(__file__).resolve().parents[2]
SDL3_RECIPE = REPO_DIR / "od-fs" / "build" / "env" / "packages" / "sdl3"
NAVIGATION_HEADER = REPO_DIR / "od-fs" / "fsapp" / "fsapp-navigation.h"
OUTPUT = REPO_DIR / "od-fs" / "Python" / "fsapp" / "surfaceapi" / "_enums_v1.py"

# Scancodes that are not keys.
SKIPPED_SCANCODES = {"RESERVED", "COUNT"}

Members = list[tuple[str, int]]


def wanted_sdl_version() -> str:
    """The SDL version of the env recipe (PACKAGE=SDL3-<version>)."""
    match = re.search(r"^PACKAGE=SDL3-([\d.]+)$", SDL3_RECIPE.read_text(), re.MULTILINE)
    if match is None:
        raise ValueError(f"no PACKAGE=SDL3-<version> in {SDL3_RECIPE}")
    return match.group(1)


def sdl_version(include_dir: Path) -> str | None:
    """The version of the SDL3 headers in include_dir, or None when it has none."""
    path = include_dir / "SDL3" / "SDL_version.h"
    if not (include_dir / "SDL3" / "SDL_keycode.h").is_file() or not path.is_file():
        return None
    text = path.read_text()
    parts = []
    for part in ("MAJOR", "MINOR", "MICRO"):
        match = re.search(rf"^#define SDL_{part}_VERSION\s+(\d+)", text, re.MULTILINE)
        if match is None:
            return None
        parts.append(match.group(1))
    return ".".join(parts)


def candidate_include_dirs() -> list[Path]:
    """The directories searched for SDL3's headers, in order."""
    env_dir = Path(os.environ.get("FSUAE_ENV_DIR") or ".build/env")
    if not env_dir.is_absolute():
        env_dir = REPO_DIR / env_dir
    candidates = sorted(env_dir.glob("*/include"))
    try:
        result = subprocess.run(
            ["pkg-config", "--variable=includedir", "sdl3"],
            stdout=subprocess.PIPE,
            stderr=subprocess.DEVNULL,
            text=True,
        )
        if result.returncode == 0 and result.stdout.strip():
            candidates.append(Path(result.stdout.strip()))
    except OSError:
        pass
    candidates += [Path("/usr/include"), Path("/usr/local/include"), Path("/opt/homebrew/include")]
    return candidates


def find_include_dir(version: str | None) -> Path | None:
    """The first searched directory with SDL3's headers of the version (any version for None)."""
    for candidate in candidate_include_dirs():
        found = sdl_version(candidate)
        if found is not None and (version is None or found == version):
            return candidate
    return None


def python_name(name: str) -> str:
    """SDL's name minus the prefix as a member name: DIGIT_<n> for the digit keys."""
    if name[0].isdigit():
        name = f"DIGIT_{name}"
    if not name.isidentifier() or keyword.iskeyword(name):
        raise ValueError(f"not a valid member name: {name}")
    return name


def parse_scancodes(text: str) -> Members:
    block = re.search(r"typedef enum SDL_Scancode\s*\{(.*?)\}\s*SDL_Scancode;", text, re.DOTALL)
    if block is None:
        raise ValueError("no SDL_Scancode enum in SDL_scancode.h")
    members = re.findall(r"^\s*SDL_SCANCODE_(\w+)\s*=\s*(\d+)", block.group(1), re.MULTILINE)
    return [(python_name(n), int(v)) for n, v in members if n not in SKIPPED_SCANCODES]


def parse_keycodes(text: str) -> Members:
    # The masks (SDLK_EXTENDED_MASK, SDLK_SCANCODE_MASK) are expressions, not literals.
    members = re.findall(r"^#define SDLK_(\w+)\s+0x([0-9a-fA-F]+)u\b", text, re.MULTILINE)
    return [(python_name(n), int(v, 16)) for n, v in members]


def parse_keymods(text: str) -> Members:
    members: Members = []
    values: dict[str, int] = {}
    pattern = r"^#define SDL_KMOD_(\w+)\s+(0x[0-9a-fA-F]+u|\(SDL_KMOD_\w+ \| SDL_KMOD_\w+\))"
    for name, value in re.findall(pattern, text, re.MULTILINE):
        if value.startswith("("):
            number = 0
            for part in re.findall(r"SDL_KMOD_(\w+)", value):
                number |= values[part]
        else:
            number = int(value.rstrip("u"), 16)
        values[name] = number
        members.append((python_name(name), number))
    return members


def parse_mouse_buttons(text: str) -> Members:
    members = re.findall(r"^#define SDL_BUTTON_(\w+)\s+(\d+)\s*$", text, re.MULTILINE)
    if not members:
        raise ValueError("no SDL_BUTTON_ values in SDL_mouse.h")
    return [(python_name(n), int(v)) for n, v in members]


def parse_navigation(text: str) -> Members:
    block = re.search(
        r"typedef enum fsapp_navigation_value\s*\{(.*?)\}\s*fsapp_navigation_value_t;",
        text,
        re.DOTALL,
    )
    if block is None:
        raise ValueError(f"no fsapp_navigation_value enum in {NAVIGATION_HEADER}")
    members = re.findall(r"^\s*FSAPP_NAVIGATION_(\w+)\s*=\s*(\d+)", block.group(1), re.MULTILINE)
    return [(python_name(n), int(v)) for n, v in members]


def write_class(lines: list[str], name: str, base: str, members: Members, hex_digits: int) -> None:
    lines += ["", "", f"class {name}({base}):"]
    for member, value in members:
        # Hex digits in upper case, as ruff format writes them.
        literal = f"0x{value:0{hex_digits}X}" if hex_digits else str(value)
        lines.append(f"    {member} = {literal}")


def generate(include_dir: Path) -> str:
    """The module's text, from the SDL3 headers in include_dir and fsapp-navigation.h."""
    version = sdl_version(include_dir)
    if version is None:
        raise ValueError(f"no SDL3 headers in {include_dir}")
    sdl_dir = include_dir / "SDL3"
    keycode_text = (sdl_dir / "SDL_keycode.h").read_text()
    lines = [
        '"""SDL3\'s key and mouse button values as typed enums, and the navigation values, of the',
        "surface API's version 1 (fsapp.surfaceapi.v1 exports them).",
        "",
        f"Generated by od-fs/tools/gen_surface_enums.py from the headers of SDL {version}",
        "(Scancode, Keycode, Keymod and MouseButton) and od-fs/fsapp/fsapp-navigation.h",
        "(Navigation). Do not edit: run the script.",
        '"""',
        "",
        "# SDL's names, I and O among them.",
        "# ruff: noqa: E741",
        "",
        "from enum import IntEnum, IntFlag",
    ]
    scancode_text = (sdl_dir / "SDL_scancode.h").read_text()
    write_class(lines, "Scancode", "IntEnum", parse_scancodes(scancode_text), 0)
    write_class(lines, "Keycode", "IntEnum", parse_keycodes(keycode_text), 8)
    write_class(lines, "Keymod", "IntFlag", parse_keymods(keycode_text), 4)
    mouse_text = (sdl_dir / "SDL_mouse.h").read_text()
    write_class(lines, "MouseButton", "IntEnum", parse_mouse_buttons(mouse_text), 0)
    write_class(lines, "Navigation", "IntEnum", parse_navigation(NAVIGATION_HEADER.read_text()), 0)
    return "\n".join(lines) + "\n"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument("--include-dir", type=Path, help="a directory with SDL3/ in it")
    parser.add_argument("--output", type=Path, default=OUTPUT, help=f"default: {OUTPUT}")
    args = parser.parse_args()
    include_dir = args.include_dir
    if include_dir is None:
        version = wanted_sdl_version()
        include_dir = find_include_dir(version)
        if include_dir is None:
            searched = ", ".join(str(path) for path in candidate_include_dirs())
            print(f"No SDL {version} headers found (searched {searched})", file=sys.stderr)
            return 1
    args.output.write_text(generate(include_dir))
    print(f"Wrote {args.output} from {include_dir}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
