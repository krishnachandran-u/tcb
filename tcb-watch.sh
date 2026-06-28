#!/usr/bin/env bash

PUSH_BIN="$(pwd)/build/tcb-push"
NOTIFY_BIN="$(pwd)/build/tcb-clipnotify"

if [ ! -f "$PUSH_BIN" ]; then
    echo "Error: build/tcb-push missing. Please build the project."
    exit 1
fi

if [ "$XDG_SESSION_TYPE" = "wayland" ]; then
    echo "tcb: Wayland environment detected. Using native wl-paste watch engine..."
    
    if ! command -v wl-paste &> /dev/null; then
        echo "Error: wl-clipboard package is required for Wayland support."
        echo "Please install it via your package manager (e.g., sudo apt install wl-clipboard)"
        exit 1
    fi

    wl-paste --primary=false --watch bash -c "wl-paste -n | \"$PUSH_BIN\""

else
    echo "tcb: X11 environment detected. Using integrated tcb-clipnotify engine..."
    
    if [ ! -f "$NOTIFY_BIN" ]; then
        echo "Error: build/tcb-clipnotify missing."
        exit 1
    fi

    if ! command -v xclip &> /dev/null; then
        echo "Error: xclip package is required for X11 support."
        echo "Please install it via your package manager (e.g., sudo apt install xclip)"
        exit 1
    fi

    while "$NOTIFY_BIN" -s clipboard; do
        xclip -selection clipboard -o 2>/dev/null | "$PUSH_BIN"
    done
fi