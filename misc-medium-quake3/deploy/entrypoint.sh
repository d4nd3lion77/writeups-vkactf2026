#!/bin/bash

set -e

if [ -z "$FLAG" ]; then
  echo "FLAG is not set"
  exit 1
fi
if [ -z "$SITE_PORT" ]; then
  echo "SITE_PORT is not set"
  exit 1
fi
if [ -z "$GAME_PORT" ]; then
  echo "GAME_PORT is not set"
  exit 1
fi
if [ -z "$IP_HOST" ]; then
  echo "IP_HOST is not set"
  exit 1
fi
export FLAG
export SITE_PORT
export GAME_PORT
export IP_HOST

cd /home/quake3

cat /home/quake3/q3aserverpack/q3a-server/autoexec.template.cfg \
  | sed "s/{{GAME_PORT}}/$GAME_PORT/g" \
  > /home/quake3/q3aserverpack/q3a-server/autoexec.cfg
rm /home/quake3/q3aserverpack/q3a-server/autoexec.template.cfg

/usr/lib/ioquake3/ioq3ded || true
rsync -a /home/quake3/q3aserverpack/baseq3/ /home/quake3/.q3a/baseq3/
rsync -a /home/quake3/q3aserverpack/q3a-server/ /home/quake3/.q3a/baseq3/

mv /home/quake3/q3aserverpack/main.py .q3a/
mv /home/quake3/q3aserverpack/templates .q3a/
rm -rf /home/quake3/q3aserverpack

python3 /home/quake3/.q3a/main.py &
cd /home/quake3/.q3a/baseq3/

exec /home/quake3/.q3a/baseq3/q3start.sh