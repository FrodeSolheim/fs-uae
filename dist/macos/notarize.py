#!/usr/bin/env python3

import os
import sys
import time
import xml.etree.ElementTree as ET

app = sys.argv[1]
app_id = sys.argv[2]

apple_id_user = os.environ.get("APPLE_ID_USER", "")
itc_provider = os.environ.get("ITC_PROVIDER", "")
if os.path.exists(".env"):
    f = open(".env", "r")
    for line in f:
        line = line.strip()
        if not line:
            continue
        key, value = line.split("=")
        os.environ[key] = value
        if key == "APPLE_ID_USER":
            apple_id_user = value
        if key == "ITC_PROVIDER":
            itc_provider = value


def shell(cmd):
    print(cmd)
    return os.popen(cmd).read()


shell("rm -f Notarize.zip")
shell("zip -r Notarize.zip \"{}\"".format(app))
result = shell("xcrun altool --notarize-app -t osx -f Notarize.zip " \
"--primary-bundle-id {} -u {} -p @env:APPLE_ID_PASS " \
"-itc_provider {} --output-format xml".format(
    app_id, apple_id_user, itc_provider))

print(result)
root = ET.fromstring(result)
dict = root.find("dict")
print(dict)
request_uuid = dict.find("dict").find("string").text
print(request_uuid)

while True:
    time.sleep(10.0)
    result = shell("xcrun altool --notarization-info {} " \
    "-u {} -p @env:APPLE_ID_PASS " \
    "-itc_provider {} --output-format xml".format(
        request_uuid, apple_id_user, itc_provider))
    if "<string>success</string>" in result:
        break
    elif "<string>in progress</string>" in result:
        print("in progress...")
        continue
    else:
        print(result)
        raise Exception("...")

print("xcrun stapler staple \"{}\"".format(app))
assert os.system("xcrun stapler staple \"{}\"".format(app)) == 0
shell("rm Notarize.zip")
