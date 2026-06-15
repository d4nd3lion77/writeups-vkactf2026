FROM debian:bookworm-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends qemu-system-x86 qemu-utils socat ca-certificates && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /challenge
COPY run-qemu.sh /challenge/run-qemu.sh
RUN mkdir -p /challenge/image && chmod +x /challenge/run-qemu.sh

EXPOSE 31337
CMD ["/challenge/run-qemu.sh"]
