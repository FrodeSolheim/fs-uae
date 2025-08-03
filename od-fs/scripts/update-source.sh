#!/bin/sh

set -e

./fs-uae spc:dump-quickstarts

python3 od-fs/scripts/update-config-table.py

python3 od-fs/scripts/update-configkey.py
python3 od-fs/scripts/update-inputevent.py
