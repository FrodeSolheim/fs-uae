#!/usr/bin/env python3
"""Gathers the files of a release from the artifacts of a dist workflow run, and writes SHA256SUMS
for them.

Usage: release_files.py --version <version> [--dev] <artifacts directory> <output directory>

The artifacts directory is laid out as actions/download-artifact leaves it when it downloads every
artifact of a run: a directory per artifact, named for the artifact. The release files are:

- the dist archive of each platform, FS-UAE-<version>-<triplet>.<extension>, from the artifact of
  the same name (as the jobs of .github/workflows/dist.yml name them);
- the source tarball, fs-uae-<version>.tar.gz (make dist names it for the version configure has,
  which for a tag is the dist version too), from the artifact named "source"
  (.github/workflows/source.yml).

They are copied to the output directory (which must not exist or be empty), with SHA256SUMS, in the
format sha256sum -c takes, with bare file names. Other artifacts are left out: the x86_64 macOS
.dmg made under Rosetta (rosetta-FS-UAE-...), and the smoke test and notarization logs of failed
jobs. A missing release file is an error, so that a release never lacks a platform.

With --dev, the files are those of the dev pre-release, which every push to main replaces: the
archives under names without the version, FS-UAE-dev-<triplet>.<extension>, so that links to them
always give the latest build, and no source tarball (see dev_release.py). SHA256SUMS then starts
with a comment line, "# FS-UAE <version>", which sha256sum -c skips.

Used by the release and dev pre-release jobs of the dist workflow; the version is the dist version
(version.py).
"""

import argparse
import hashlib
import re
import shutil
import sys
from pathlib import Path

from check import ARCHIVE_EXTENSIONS, os_for_triplet

# The platforms a release has a dist for.
TRIPLETS = [
    "x86_64-linux-gnu",
    "aarch64-linux-gnu",
    "x86_64-w64-mingw32",
    "arm64-apple-darwin",
    "x86_64-apple-darwin",
]
SOURCE_ARTIFACT = "source"
CHECKSUMS = "SHA256SUMS"
# What the archives of the dev pre-release have in their names instead of the version.
DEV_NAME = "dev"


class ReleaseFilesError(Exception):
    def __init__(self, *messages: str) -> None:
        super().__init__(*messages)
        self.messages = list(messages)


def release_files(version: str, dev: bool = False) -> dict[str, tuple[str, str]]:
    """The files of a release of the version (or of the dev pre-release), by name, each with the
    artifact it is in and its name there."""
    # A dist version starts with the AC_INIT version (<major>.<minor>.<revision>, see version.py).
    if re.match(r"\d+\.\d+\.\d+", version) is None:
        raise ReleaseFilesError(f"not a dist version: {version}")
    files = {}
    for triplet in TRIPLETS:
        extension = ARCHIVE_EXTENSIONS[os_for_triplet(triplet) or ""]
        artifact = f"FS-UAE-{version}-{triplet}{extension}"
        name = f"FS-UAE-{DEV_NAME}-{triplet}{extension}" if dev else artifact
        files[name] = (artifact, artifact)
    if not dev:
        tarball = f"fs-uae-{version}.tar.gz"
        files[tarball] = (SOURCE_ARTIFACT, tarball)
    return files


def sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as f:
        for block in iter(lambda: f.read(1024 * 1024), b""):
            digest.update(block)
    return digest.hexdigest()


def gather(version: str, artifacts_dir: Path, out_dir: Path, dev: bool = False) -> list[str]:
    """Copies the release files to out_dir, writes SHA256SUMS there, and returns the names of the
    files, SHA256SUMS last."""
    files = release_files(version, dev)
    missing = [
        f"missing {source} (artifact {artifact})"
        for artifact, source in files.values()
        if not (artifacts_dir / artifact / source).is_file()
    ]
    if missing:
        found = (
            sorted(path.name for path in artifacts_dir.iterdir()) if artifacts_dir.is_dir() else []
        )
        raise ReleaseFilesError(*missing, f"the artifacts are: {', '.join(found) or 'none'}")
    # Anything already there would end up in the release too.
    if out_dir.exists() and (not out_dir.is_dir() or any(out_dir.iterdir())):
        raise ReleaseFilesError(f"{out_dir} is not an empty directory")
    out_dir.mkdir(parents=True, exist_ok=True)
    for name, (artifact, source) in files.items():
        shutil.copyfile(artifacts_dir / artifact / source, out_dir / name)
    names = sorted(files)
    with (out_dir / CHECKSUMS).open("w") as f:
        # The dev archives' names have no version in them. sha256sum -c skips comment lines.
        if dev:
            f.write(f"# FS-UAE {version}\n")
        for name in names:
            f.write(f"{sha256(out_dir / name)}  {name}\n")
    return [*names, CHECKSUMS]


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--version", required=True, help="the dist version (version.py)")
    parser.add_argument(
        "--dev",
        action="store_true",
        help="the files of the dev pre-release: stable names, no source tarball",
    )
    parser.add_argument("artifacts_dir", type=Path, help="one directory per artifact")
    parser.add_argument("out_dir", type=Path, help="where the release files go")
    args = parser.parse_args()
    try:
        names = gather(args.version, args.artifacts_dir, args.out_dir, args.dev)
    except ReleaseFilesError as e:
        for message in e.messages:
            print(f"Error: {message}", file=sys.stderr)
        return 1
    for name in names:
        print(name)
    return 0


if __name__ == "__main__":
    sys.exit(main())
