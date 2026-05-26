#!/usr/bin/env bash
set -e

ENV="LEDController"

PORTS=(
  "192.168.100.240"
  "192.168.100.241"
  "192.168.100.242"
)

echo "Building firmware once..."
pio run -e "$ENV"

echo "Uploading in parallel..."

for PORT in "${PORTS[@]}"; do
  echo "Uploading to $PORT..."
  pio run -e "$ENV" -t upload -t nobuild --upload-port "$PORT" &
done

wait

echo "All uploads finished."