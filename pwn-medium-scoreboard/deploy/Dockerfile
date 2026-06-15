FROM alpine:3.22

RUN apk add --no-cache \
    qemu-system-x86_64 \
    socat \
    cpio \
    gzip \
    coreutils \
    util-linux \
    bash

RUN adduser -D -h /home/vkactf -s /bin/bash vkactf

WORKDIR /home/vkactf

COPY . .

RUN chmod +x *.sh && \
    chmod 644 initramfs.cpio.gz scoreboard.ko bzImage && \
    chown -R vkactf:vkactf /home/vkactf

EXPOSE 1337

USER vkactf

CMD ["socat", "TCP-LISTEN:1337,reuseaddr,fork", "EXEC:./run.sh,stderr"]