#!/bin/bash

if [[ $# -ne 1 ]]; then
  echo "USAGE: sync.sh <remote>"
  exit 2
fi

remote="$1"

rsync -a -e ssh --exclude='*.swp' --progress *.{cc,h} scripts Makefile "$remote":needletail
rsync -a -e ssh --progress "$remote":needletail/*.out* .
rsync -a -e ssh --progress "$remote":needletail/needletail needletail."$remote"
