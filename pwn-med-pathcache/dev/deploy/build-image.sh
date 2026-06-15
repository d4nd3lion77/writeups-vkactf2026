#!/bin/sh
set -eu

ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)"
ISO="${1:-${WINDOWS_ISO:-}}"
IMAGE="$ROOT/deploy/image/vstor_cache.qcow2"
SEED_DIR="$ROOT/deploy/image/seed"
SEED_ISO="$ROOT/deploy/image/vstor_seed.iso"
PAYLOAD="$SEED_DIR/payload"
BUILD="$ROOT/build/x64/Release"
UNATTEND="${UNATTEND:-$ROOT/guest/Autounattend.servercore.xml}"

if [ ! -f "$UNATTEND" ]; then
  UNATTEND="$ROOT/guest/Autounattend.xml"
fi

if [ -z "$ISO" ]; then
  echo "usage: $0 /path/to/windows.iso" >&2
  echo "or set WINDOWS_ISO=/path/to/windows.iso" >&2
  exit 2
fi

if [ ! -f "$ISO" ]; then
  echo "missing Windows ISO: $ISO" >&2
  exit 1
fi

for f in VStorCache.sys VStorCache.inf VStorCacheTest.cer chall.exe; do
  if [ ! -f "$BUILD/$f" ]; then
    echo "missing build artifact: $BUILD/$f" >&2
    echo "run ./build.ps1 and ./sign.ps1 from Windows first" >&2
    exit 1
  fi
done

LAUNCHER="$ROOT/third_party/appjaillauncher-rs/target/release/appjaillauncher-rs.exe"
if [ ! -f "$LAUNCHER" ]; then
  echo "missing AppJailLauncher: $LAUNCHER" >&2
  echo "run: cd ../third_party/appjaillauncher-rs && cargo build --release" >&2
  exit 1
fi

ISO_TOOL="$(command -v genisoimage || command -v mkisofs || true)"

rm -rf "$SEED_DIR"
mkdir -p "$PAYLOAD" "$ROOT/deploy/image"
cp "$UNATTEND" "$SEED_DIR/Autounattend.xml"
cp "$ROOT/guest/setup.ps1" "$SEED_DIR/setup.ps1"
cp "$ROOT/guest/VStorSvcTask.xml" "$SEED_DIR/VStorSvcTask.xml"
cp "$ROOT/guest/sync-flag.ps1" "$SEED_DIR/sync-flag.ps1"
cp "$ROOT/guest/FlagSyncTask.xml" "$SEED_DIR/FlagSyncTask.xml"
cp "$BUILD/VStorCache.sys" "$BUILD/VStorCache.inf" "$BUILD/VStorCacheTest.cer" "$BUILD/chall.exe" "$PAYLOAD/"
cp "$LAUNCHER" "$PAYLOAD/"

SEED_ARGS="-drive file=fat:rw:$SEED_DIR,format=raw,media=disk"
if [ -n "$ISO_TOOL" ]; then
  "$ISO_TOOL" -quiet -J -r -V VSTORSEED -o "$SEED_ISO" "$SEED_DIR"
  SEED_ARGS="-drive file=$SEED_ISO,media=cdrom,if=ide,index=3"
fi

qemu-img create -f qcow2 "$IMAGE" "${DISK_SIZE:-32G}"

exec qemu-system-x86_64 \
  -m "${QEMU_RAM:-4096}" \
  -smp "${QEMU_SMP:-2}" \
  -accel tcg \
  -cpu qemu64 \
  -drive "file=$IMAGE,if=ide,format=qcow2,cache=writeback" \
  -cdrom "$ISO" \
  $SEED_ARGS \
  -boot order=d \
  -device e1000,netdev=n0 \
  -netdev user,id=n0 \
  -display "${QEMU_DISPLAY:-none}" \
  -serial mon:stdio
