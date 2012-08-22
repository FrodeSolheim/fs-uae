from __future__ import division
from __future__ import print_function
from __future__ import absolute_import
from __future__ import unicode_literals

import os
import sys
from .fs import memoize

def get_real_case(path):
    """Check the case for the (case insensitive) path. Used to make the
    database portable across sensitive/insensitive file systems."""

    # not really needed on Linux

    parts = []
    drive, p = os.path.splitdrive(path)
    if path.startswith("/"):
        drive = "/"
    elif drive:
        # on Windows, add / to make drive a valid path
        drive = drive + "/"
    last = ""
    while p != last:
        name = os.path.basename(p).lower()
        if not name:
            break
        parts.append(os.path.basename(p).lower())
        last = p
        p = os.path.dirname(p)
    parts.reverse()
    #print(drive, parts)
    result = [drive]
    result.extend(parts)

    combined = drive
    combined = combined.upper()
    k = 1
    for part in parts:
        #print("part is", part)
        if os.path.isdir(combined):
            #print("checking case of", combined+ "/" + part)
            for name in os.listdir(combined):
                if name.lower() == part:
                    #print("found case =", name)
                    combined += "/" + name
                    result[k] = name
                    break
            else:
                raise Exception("could not find case for path " + path)
        k += 1
    # normalizing slashes to forward slash to make the database more
    # portable
    result_path = os.path.join(*result).replace("\\", "/")
    return result_path
