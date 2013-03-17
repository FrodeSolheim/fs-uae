#!/bin/bash

SERIES=`cat SERIES`
mkdir -p dist/$SERIES/po/fs-uae
find . -name *.mo -delete
python po/update.py &> dist/$SERIES/po/status.txt
