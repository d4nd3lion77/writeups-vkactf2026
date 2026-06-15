#!/bin/bash

set -eux

./decompress.sh

cp ./exploit initramfs/exploit
chmod +x initramfs/exploit

./compress.sh

qemu-system-x86_64 \
    -kernel bzImage \
    -initrd initramfs.cpio.gz \
    -append "console=ttyS0 earlyprintk=serial nokaslr oops=panic panic=1 quiet" \
    -m 256M \
    -smp 1 \
    -cpu qemu64 \
    -nographic \
    -monitor none \
    -no-reboot \
    -snapshot \
    -s