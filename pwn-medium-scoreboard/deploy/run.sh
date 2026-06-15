#!/bin/bash
set -euo pipefail

ulimit -c 0

echo "Send base64 encoded exploit and finish with __EOF__"

{
    while IFS= read -r line; do
        [ "$line" = "__EOF__" ] && break
        echo "$line"
    done
} > exploit.b64

base64 -d exploit.b64 > exploit

size="$(stat -c%s exploit)"
[ "$size" -le 2097152 ] || {
    echo "exploit too large"
    exit 1
}

chmod +x exploit

./decompress.sh

rm -f initramfs/exploit
cp exploit initramfs/exploit

chmod u+w initramfs/flag 2>/dev/null || true
rm -f initramfs/flag

printf '%s\n' "${FLAG:-not_flag}" > initramfs/flag
chmod 400 initramfs/flag
export FLAG=not_flag

./compress.sh

timeout 180 qemu-system-x86_64 \
  -kernel bzImage \
  -initrd initramfs.cpio.gz \
  -append "console=ttyS0 earlyprintk=serial nokaslr oops=panic panic=1 quiet" \
  -m 64M \
  -smp 1 \
  -cpu qemu64 \
  -nographic \
  -monitor none \
  -serial stdio \
  -no-reboot