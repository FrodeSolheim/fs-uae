#!/bin/sh

./fs-uae spc:dump-quickstarts
python3 od-fs/scripts/update-configkey.py
python3 od-fs/scripts/update-inputevent.py
