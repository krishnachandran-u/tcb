#!/usr/bin/env bash
set -e

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

mkdir -p build && cd build
cmake ..
make
sudo make install

cd "$PROJECT_DIR"
mkdir -p "$HOME/.config/systemd/user/"

rm -f "$HOME/.config/systemd/user/tcb.service"
rm -f "$HOME/.config/systemd/user/tcb-watch.service"

cp tcb.service "$HOME/.config/systemd/user/tcb.service"
cp tcb-watch.service "$HOME/.config/systemd/user/tcb-watch.service"

systemctl --user daemon-reload

echo "Installation successful! Run 'tcb' to begin."