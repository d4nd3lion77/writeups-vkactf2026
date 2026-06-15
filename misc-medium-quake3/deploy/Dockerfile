FROM debian:bookworm-slim

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y \
        python3 python3-pip \
        ioquake3-server \
        rsync && \
    rm -rf /var/lib/apt/lists/*

RUN useradd -m -s /bin/bash quake3

WORKDIR /home/quake3

COPY q3aserverpack ./q3aserverpack

RUN pip3 install --no-cache-dir Flask --break-system-packages

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

RUN chown -R quake3:quake3 /home/quake3

USER quake3

ENTRYPOINT ["/entrypoint.sh"]

# docker run -d \
#   -e FLAG="vka{example}" \
#   -e SITE_PORT=2222 \
#   -e GAME_PORT=22244 \
#   -e IP_HOST="85.143.221.172 ПОСТАВИТЬ СВОЙ" \
#   -p 2222:2222 \
#   -p 22244:22244/udp \
#   quake3-image