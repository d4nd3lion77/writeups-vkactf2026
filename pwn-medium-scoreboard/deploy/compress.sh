#!/bin/bash
set -euo pipefail

(
    cd initramfs
    find . -print0 \
        | cpio --null -ov --format=newc --owner=0:0 \
        | gzip -9
) > initramfs.cpio.gz