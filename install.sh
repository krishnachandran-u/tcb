#!/usr/bin/env bash
set -e

# sudo rm -f /usr/lib/systemd/user/tcb.service
# sudo rm -f /usr/local/share/systemd/user/tcb.service

PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$PROJECT_DIR"

mkdir -p build && cd build
cmake ..
make
sudo make install

cd "$PROJECT_DIR"

rm -f "$HOME/.config/systemd/user/tcb.service"

mkdir -p "$HOME/.config/systemd/user/"
cp tcb.service "$HOME/.config/systemd/user/tcb.service"

systemctl --user daemon-reload
# systemctl --user reset-failed tcb

echo "--------------------------------------------------"
echo "Success! Run 'tcb start' to begin manual control."
echo "--------------------------------------------------"