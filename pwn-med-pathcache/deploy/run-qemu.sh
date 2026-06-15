#!/bin/sh
set -eu

ACCEL="tcg,thread=multi"
CPU="qemu64"
if [ -e /dev/kvm ]; then
  ACCEL="kvm"
  CPU="host"
fi

BASE_IMAGE="${QEMU_IMAGE:-/challenge/image/vstor_cache.qcow2}"
if [ ! -f "$BASE_IMAGE" ]; then
  echo "missing VM image: $BASE_IMAGE" >&2
  exit 1
fi

IMAGE="${QEMU_WORK_IMAGE:-/tmp/vstor_cache.qcow2}"
if [ ! -f "$IMAGE" ]; then
  cp "$BASE_IMAGE" "$IMAGE"
fi

exec qemu-system-x86_64 \
  -m "${QEMU_RAM:-2048}" \
  -smp "${QEMU_SMP:-2}" \
  -accel "$ACCEL" \
  -cpu "$CPU" \
  -drive "file=$IMAGE,if=ide,format=qcow2,cache=writeback" \
  -netdev "user,id=n0,hostfwd=tcp::31337-:31337" \
  -device e1000,netdev=n0 \
  -display none \
  -serial mon:stdio \
  ${QEMU_EXTRA:-}
