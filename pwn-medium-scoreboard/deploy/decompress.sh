#!/bin/bash
set -e

if [ ! -f initramfs.cpio.gz ]; then
    echo "Error: initramfs.cpio.gz not found"
    exit 1
fi

rm -rf initramfs
mkdir -p initramfs

gzip -d -c initramfs.cpio.gz \
    | cpio -idmv --no-absolute-filenames \
    -D initramfs