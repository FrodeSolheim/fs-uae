#!/usr/bin/env python3

import hashlib
import os
import sys
from typing import Any


def verify(archive: str, h: Any, checksum: str) -> bool:
    with open(archive, "rb") as f:
        actual_checksum = h(f.read()).hexdigest()
    result = actual_checksum == checksum
    if result:
        print("Checksum verified")
    else:
        print("Checksum verification failed")
        print("Expected", checksum)
        print("But got:", actual_checksum)
    return result


def main():
    build_env_dir = os.environ["BUILD_ENV_DIR"]
    url = sys.argv[1]
    checksum = sys.argv[2]

    if len(checksum) == 64:
        h = hashlib.sha256
        checksum = checksum
    elif checksum.startswith("sha256:"):
        h = hashlib.sha256
        checksum = checksum[7:]
    else:
        raise Exception("Unknown hash function")

    archive = url.split("/")[-1]
    if not os.path.exists(f"{build_env_dir}/_sources"):
        os.makedirs(f"{build_env_dir}/_sources")
    archive = os.path.join(f"{build_env_dir}/_sources", archive)

    if os.path.exists(archive):
        if verify(archive, h, checksum):
            sys.exit(0)
        print("Removing archive", archive)
        os.remove(archive)

    # FIXME: Replace use of wget, just use python instead
    # if os.system(f'cd {build_env_dir}/_sources && curl "{url}" -o {archive}') != 0:

    print("Using --no-check-certificate due to missing certificates in the build process")
    print("The downloads are verified using SHA-256")

    if os.system(f'cd {build_env_dir}/_sources && wget --no-check-certificate "{url}"') != 0:
        print("Failed to download")
        sys.exit(1)

    if not verify(archive, h, checksum):
        sys.exit(2)


if __name__ == "__main__":
    main()
