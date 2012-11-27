#!/bin/bash

SERIES=`cat SERIES`
mkdir -p dist/$SERIES/po/fs-uae
python po/update.py &> dist/$SERIES/po/status.txt
