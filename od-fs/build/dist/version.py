#!/usr/bin/env python3
"""Prints the version a dist or build is made for, from configure.ac and git.

Usage: version.py [--commit] [<project directory>]

The version is the AC_INIT version in configure.ac, for a tag build, or a development version
made from it and the commit:

- Tag build: the commit has the tag v<version>. A commit with another v* tag (and none for the
  version) fails, so that a release cannot be made with the wrong version.
- Other builds: <version>-dev.<YYYYMMDD>T<HHMM>+g<hash>, for example
  5.0.8-dev.20261111T0930+g0123456789, where the date and time (to the minute) are the commit's
  committer date in UTC, and hash is the commit's abbreviated hash (10 or more digits). It is made
  from the commit alone, so the same commit gets the same version wherever and by whoever it is
  built, also in a shallow clone (no commit count, which would need the history and differ between
  repositories). Who made a build is its origin (FSUAE_BUILD_ORIGIN in configure.ac), not its
  version.

The date and time are one SemVer pre-release identifier, with a "T" between them as in ISO 8601,
so that the time can keep its leading zero (a numeric identifier may not have one, 0930) and the
identifier is not a large number (some SemVer tools take numeric identifiers as 32-bit integers).
Being of fixed width, it sorts in commit (date) order in SemVer, where alphanumeric identifiers
compare as text, and in dpkg and RPM (the package version has "~" for "-", see obs/deb-src.sh),
which compare the digits on either side of the "T" as numbers; every development version sorts
before its release (<version>), and after the earlier development versions <version>-dev.<count>
(SemVer puts alphanumeric identifiers after numeric ones). The build metadata (+g<hash>) does not
count in SemVer: two commits in the same minute differ only by their hash.

The dist entry script (dist, in this directory) runs it first, passes the version to configure
(FSUAE_VERSION), and checks that the archive name and the program have it (check.py). configure
also runs it for builds from a git checkout that do not set FSUAE_VERSION (development builds), and
falls back to the AC_INIT version when it fails. It also runs on a Mac before the env prefix with
its Python is set up, so it must work with the macOS system Python (3.9).

With --commit, it prints the whole hash of the checked-out commit instead, which the entry script
and configure pass on as FSUAE_BUILD_COMMIT for the program's build header. (So that a pull request
build names the pull request's own commit, the workflow checks that commit out, rather than
GitHub's merge commit.)
"""

import argparse
import re
import subprocess
import sys
from datetime import datetime, timezone
from pathlib import Path

# The tags of releases, the ones that make a build a tag build.
TAG_PATTERN = "v*"


class VersionError(Exception):
    pass


def git(project_dir: Path, *args: str) -> str:
    try:
        result = subprocess.run(
            ["git", *args], cwd=project_dir, capture_output=True, text=True, check=False
        )
    except OSError as e:
        raise VersionError(f"cannot run git: {e}") from e
    if result.returncode != 0:
        raise VersionError(
            f"git {' '.join(args)} failed in {project_dir} (the dist version is made with git, "
            f"so the dist must be made from a git checkout): {result.stderr.strip()}"
        )
    return result.stdout.strip()


def ac_init_version(project_dir: Path) -> str:
    """The version in AC_INIT in configure.ac."""
    path = project_dir / "configure.ac"
    try:
        text = path.read_text()
    except OSError as e:
        raise VersionError(f"cannot read {path}: {e}") from e
    match = re.search(r"^AC_INIT\(\[FS-UAE\],\s*\[(\d+\.\d+\.\d+)\]", text, re.MULTILINE)
    if match is None:
        raise VersionError(f"no AC_INIT([FS-UAE], [<major>.<minor>.<revision>], ...) in {path}")
    return match.group(1)


def dist_version(project_dir: Path) -> str:
    version = ac_init_version(project_dir)
    tags = git(project_dir, "tag", "--points-at", "HEAD", "--list", TAG_PATTERN).split()
    tagged = f"v{version}" in tags
    if tags and not tagged:
        raise VersionError(
            f"the commit is tagged {', '.join(tags)}, but the version in configure.ac (AC_INIT) "
            f"is {version}: a tag build needs the tag v{version}"
        )
    if tagged:
        return version
    # The committer date (not the author date: a rebased or cherry-picked commit is a new commit),
    # and the hash with 10 digits or more, so that a fresh clone, where git may pick another default
    # length, names the same commit the same way.
    timestamp, commit_hash = git(project_dir, "log", "-1", "--abbrev=10", "--format=%ct %h").split()
    date = datetime.fromtimestamp(int(timestamp), timezone.utc)
    return f"{version}-dev.{date:%Y%m%dT%H%M}+g{commit_hash}"


def commit(project_dir: Path) -> str:
    """The whole hash of the checked-out commit."""
    return git(project_dir, "rev-parse", "--verify", "HEAD")


def main() -> int:
    parser = argparse.ArgumentParser(description="Prints the version a dist or build is made for.")
    parser.add_argument(
        "--commit", action="store_true", help="print the checked-out commit's whole hash instead"
    )
    parser.add_argument(
        "project_dir",
        nargs="?",
        type=Path,
        default=Path(__file__).resolve().parents[3],
        help="the project directory (default: the one this script is in)",
    )
    args = parser.parse_args()
    try:
        print(commit(args.project_dir) if args.commit else dist_version(args.project_dir))
    except VersionError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
