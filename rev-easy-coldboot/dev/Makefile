CC ?= gcc
MIPS_CC ?= mips-linux-gnu-gcc
CFLAGS ?= -O2 -Wall -Wextra -std=c99

.PHONY: all native mips dist clean

all: native

native: build/watchdogd.exe

build/watchdogd.exe: src/watchdogd.c src/generated_secrets.h
	$(CC) $(CFLAGS) -o $@ src/watchdogd.c

mips: dist/rootfs/sbin/watchdogd

dist/rootfs/sbin/watchdogd: src/watchdogd.c src/generated_secrets.h
	$(MIPS_CC) $(CFLAGS) -s -static -o $@ src/watchdogd.c

dist:
	python tools/gen_dist.py

clean:
	rm -f build/watchdogd.exe dist/rootfs/sbin/watchdogd
