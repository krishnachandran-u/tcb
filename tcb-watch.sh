#!/usr/bin/env bash

PUSH_BIN="tcb-push"
NOTIFY_BIN="tcb-clipnotify"

tcb-daemon &

if ! command -v "$PUSH_BIN" &> /dev/null; then
    echo "Error: $PUSH_BIN not found in PATH. Please run 'sudo make install'."
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
    
    # Verify that clipnotify is available globally
    if ! command -v "$NOTIFY_BIN" &> /dev/null; then
        echo "Error: $NOTIFY_BIN not found in PATH. Please run 'sudo make install'."
        exit 1
    fi

    if ! command -v xclip &> /dev/null; then
        echo "Error: xclip package is required for X11 support."
        echo "Please install it via your package manager (e.g., sudo apt install xclip)"
        exit 1
    fi

    while "$NOTIFY_BIN" -s clipboard; do
        CLIP_TARGETS=$(xclip -selection clipboard -t TARGETS -o 2>/dev/null)

        if echo "$CLIP_TARGETS" | grep -q "text/uri-list"; then
            continue
        fi

        xclip -selection clipboard -o 2>/dev/null | "$PUSH_BIN"
    done
fi