#!/usr/bin/env python3
"""Replaces the files of the rolling "dev" pre-release on GitHub with those of a newer build of
main, so that the latest development build can be downloaded without a GitHub account, from links
that stay the same (https://github.com/<owner>/<repo>/releases/download/dev/<name>).

Usage: dev_release.py --commit <commit> --version <version> <directory>

The directory has the files of the build of the commit and their SHA256SUMS
(release_files.py --dev, in this directory, gathers them, with names without the version), and the
version is its dist version (version.py). Every file in it is uploaded. Runs in the git checkout of
the commit (with the whole history) and calls gh (the GitHub CLI) for the repository GH_REPO, with
the token in GH_TOKEN. Used by the dev release job of .github/workflows/dist.yml, for pushes to
main.

The pre-release is one long-lived GitHub release of the tag "dev" (not v*, so version.py does not
take a build of it for a release build), marked as a pre-release and never as the latest release.
It is kept and updated rather than deleted and made again for every build: the links are the same
either way, but a new release would notify everyone who watches the repository's releases on every
push, and would leave no release at all between deleting the old one and publishing the new one.
(The download counts are per file, so they start again with every build either way.) The first run
makes it (and the tag). Every run then:

1. Does nothing if the tag is already at a newer commit: the dists of an earlier push can finish
   after those of a later one. The checkout is made when the job starts, after the previous dev
   release job (the job's concurrency group), so it has the tag's commit, and the commits of main
   up to this one; the commit is older if it is an ancestor of the tag's. (After a force-push to
   main, a build of a commit that is no longer on main is not recognized as older.)
2. Uploads the files under temporary names (incoming-<name>), first deleting any that an earlier,
   failed run left. A failure here (most of the time is spent uploading) leaves the previous build
   as it was.
3. Swaps them in: for each file, SHA256SUMS last, deletes the asset of that name and renames the
   uploaded one to it; then deletes the assets that the new build does not have. Each of these is a
   quick API call, but for those few seconds a download can get a file of the previous build, or
   none. A run that stops here (cancelled by hand, or an API error) leaves a mix of the two builds,
   which sha256sum -c SHA256SUMS shows, until the next run, or the job run again.
4. Moves the tag to the commit and updates the title and notes, which name the version and the
   commit.
"""

import argparse
import json
import os
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path
from typing import Any

from release_files import CHECKSUMS, sha256

DEV_TAG = "dev"
# The prefix of the files of a build while they are uploaded, before they replace the previous
# build's.
INCOMING_PREFIX = "incoming-"


class DevReleaseError(Exception):
    pass


class GitHub:
    """The repository on GitHub, through gh."""

    def __init__(self, repo: str) -> None:
        self.repo = repo

    def run(self, *args: str, missing_ok: bool = False) -> str | None:
        """Runs gh and returns its output, or None for "404 Not Found" when missing_ok is set."""
        result = subprocess.run(["gh", *args], capture_output=True, text=True, check=False)
        if result.returncode != 0:
            if missing_ok and "(HTTP 404)" in result.stderr:
                return None
            raise DevReleaseError(f"gh {' '.join(args[:4])} failed: {result.stderr.strip()}")
        return result.stdout

    def get(self, endpoint: str, missing_ok: bool = False) -> Any:
        """Gets repos/<repo>/<endpoint> from the REST API (None if it is not found and missing_ok
        is set)."""
        output = self.run("api", f"repos/{self.repo}/{endpoint}", missing_ok=missing_ok)
        return None if output is None else json.loads(output)

    def change(
        self, method: str, endpoint: str, fields: dict[str, str | bool] | None = None
    ) -> Any:
        """Calls the REST API with the method (POST, PATCH or DELETE) on repos/<repo>/<endpoint>,
        and returns the response (None if empty)."""
        args = ["api", "-X", method, f"repos/{self.repo}/{endpoint}"]
        for key, value in (fields or {}).items():
            # -F makes true and false JSON booleans; -f passes a string as it is.
            if isinstance(value, bool):
                args += ["-F", f"{key}={str(value).lower()}"]
            else:
                args += ["-f", f"{key}={value}"]
        output = self.run(*args)
        return json.loads(output) if output and output.strip() else None

    def upload(self, tag: str, paths: list[Path]) -> None:
        self.run("release", "upload", tag, *(str(path) for path in paths))


def check_files(directory: Path) -> list[Path]:
    """The files in the directory, SHA256SUMS last, after checking that SHA256SUMS lists every other
    file with its checksum."""
    checksums = directory / CHECKSUMS
    if not checksums.is_file():
        raise DevReleaseError(f"{directory} has no {CHECKSUMS}")
    listed = {}
    for line in checksums.read_text().splitlines():
        if line.startswith("#"):
            continue
        digest, _, name = line.partition("  ")
        listed[name] = digest
    present = {path.name for path in directory.iterdir() if path.is_file()} - {CHECKSUMS}
    if set(listed) != present:
        raise DevReleaseError(f"the files in {directory} are not the ones {CHECKSUMS} lists")
    for name, digest in listed.items():
        if sha256(directory / name) != digest:
            raise DevReleaseError(f"{name} in {directory} does not match {CHECKSUMS}")
    return [*(directory / name for name in sorted(present)), checksums]


def git_succeeds(*args: str) -> bool:
    return subprocess.run(["git", *args], capture_output=True, check=False).returncode == 0


def tag_is_newer(commit: str, tag_commit: str) -> bool:
    """Whether the commit of the tag is newer than the commit, which then must not replace it. A
    tag commit that is not in the checkout is taken to be newer (it would have been pushed after the
    checkout was made)."""
    if not git_succeeds("cat-file", "-e", f"{tag_commit}^{{commit}}"):
        return True
    return commit != tag_commit and git_succeeds("merge-base", "--is-ancestor", commit, tag_commit)


def title_and_notes(version: str, commit: str, repo: str) -> tuple[str, str]:
    server = os.environ.get("GITHUB_SERVER_URL", "https://github.com")
    link = f"[{commit[:10]}]({server}/{repo}/commit/{commit})"
    notes = (
        f"FS-UAE {version}, a development build of {link} on `main`. It is not a release.\n\n"
        "Every push to `main` replaces the files with those of its build, under the same names, "
        "so links to them always give the latest build. The files can be checked with "
        "`sha256sum -c SHA256SUMS`; its first line names the version.\n\n"
        'There is no source tarball (`make dist`); the "Source code" archives of this '
        "pre-release are of the commit."
    )
    return f"FS-UAE {version} (development build)", notes


def replace_files(github: GitHub, commit: str, version: str, directory: Path) -> str:
    """Replaces the files of the dev pre-release with those in the directory, and returns what was
    done."""
    files = check_files(directory)
    tag = github.get(f"git/ref/tags/{DEV_TAG}", missing_ok=True)
    if tag is not None and tag_is_newer(commit, tag["object"]["sha"]):
        return (
            f"The {DEV_TAG} pre-release is of {tag['object']['sha']}, which is newer than "
            f"{commit}; it is left as it is."
        )
    title, notes = title_and_notes(version, commit, github.repo)
    release = github.get(f"releases/tags/{DEV_TAG}", missing_ok=True)
    if release is None:
        release = github.change(
            "POST",
            "releases",
            {
                "tag_name": DEV_TAG,
                "target_commitish": commit,
                "name": title,
                "body": notes,
                "prerelease": True,
                "make_latest": "false",
            },
        )
    release_id = release["id"]

    # Files a failed run left.
    for asset in release["assets"]:
        if asset["name"].startswith(INCOMING_PREFIX):
            github.change("DELETE", f"releases/assets/{asset['id']}")
    with tempfile.TemporaryDirectory(prefix="dev-release-") as temp:
        incoming = []
        for path in files:
            incoming.append(Path(temp) / f"{INCOMING_PREFIX}{path.name}")
            shutil.copyfile(path, incoming[-1])
        github.upload(DEV_TAG, incoming)

    assets = {
        asset["name"]: asset["id"] for asset in github.get(f"releases/{release_id}")["assets"]
    }
    missing = [path.name for path in incoming if path.name not in assets]
    if missing:
        raise DevReleaseError(f"the release does not have the uploaded {', '.join(missing)}")
    for path in files:
        if path.name in assets:
            github.change("DELETE", f"releases/assets/{assets.pop(path.name)}")
        github.change(
            "PATCH",
            f"releases/assets/{assets.pop(INCOMING_PREFIX + path.name)}",
            {"name": path.name},
        )
    # Those of the previous build that this one does not have.
    for asset_id in assets.values():
        github.change("DELETE", f"releases/assets/{asset_id}")

    github.change("PATCH", f"git/refs/tags/{DEV_TAG}", {"sha": commit, "force": True})
    github.change(
        "PATCH",
        f"releases/{release_id}",
        {
            "name": title,
            "body": notes,
            "draft": False,
            "prerelease": True,
            "make_latest": "false",
        },
    )
    return f"The {DEV_TAG} pre-release now has the files of {version} ({commit})."


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--commit", required=True, help="the commit the files were made from")
    parser.add_argument("--version", required=True, help="the dist version (version.py)")
    parser.add_argument("directory", type=Path, help="the files, with SHA256SUMS")
    args = parser.parse_args()
    repo = os.environ.get("GH_REPO")
    if not repo:
        print("Error: GH_REPO (<owner>/<repository>) is not set", file=sys.stderr)
        return 2
    try:
        print(replace_files(GitHub(repo), args.commit, args.version, args.directory))
    except DevReleaseError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
