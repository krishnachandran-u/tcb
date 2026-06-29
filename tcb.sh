#!/usr/bin/env bash

show_help() {
printf '\e[35m'
cat <<'EOF'

           __        ___.             
  ______ _/  |_  ____\_ |__    ______ 
 /_____/ \   __\/ ___\| __ \  /_____/ 
 /_____/  |  | \  \___| \_\ \ /_____/ 
          |__|  \___  >___  /         
                    \/    \/          

EOF
printf '%b' '\e[0m'
    echo "Usage: tcb [start|stop|restart|status|logs|auto|sync]"
    echo ""
    echo "Commands:"
    echo "  start           Start the core storage daemon engine"
    echo "  stop            Stop all tcb infrastructure services"
    echo "  restart         Restart all tcb infrastructure services"
    echo "  status          Check the running health states of your infrastructure"
    echo "  logs            Tail real-time operational messages"
    echo "  auto [on|off]   Toggle automatic clipboard monitoring"
    echo "  sync            Manually capture current clipboard content (alias: fire, f)"
    exit 1
}

if [ -z "$1" ]; then
    show_help
fi

case "$1" in
    start)
        systemctl --user start tcb
        echo "TCB Core Storage Daemon started."
        ;;
    stop)
        systemctl --user stop tcb-watch tcb 2>/dev/null || true
        echo "All TCB services stopped."
        ;;
    restart)
        systemctl --user restart tcb
        if systemctl --user is-active --quiet tcb-watch; then
            systemctl --user restart tcb-watch
        fi
        echo "TCB infrastructure restarted."
        ;;
    status)
        echo "=== Core Storage Daemon ==="
        systemctl --user status tcb --no-pager || true
        echo ""
        echo "=== Automatic Clipboard Monitor ==="
        systemctl --user status tcb-watch --no-pager || true
        ;;
    logs)
        journalctl --user -u tcb -u tcb-watch -f
        ;;
    auto)
        if [ "$2" = "on" ]; then
            systemctl --user start tcb-watch
            echo "Automatic clipboard capturing enabled."
        elif [ "$2" = "off" ]; then
            systemctl --user stop tcb-watch
            echo "Automatic clipboard capturing disabled. (Manual mode active)"
        else
            echo "Usage: tcb auto [on|off]"
            exit 1
        fi
        ;;
    sync|fire|f)
        if ! systemctl --user is-active --quiet tcb; then
            echo "Error: Core storage daemon isn't running. Run 'tcb start' first."
            exit 1
        fi
        
        if [ "$XDG_SESSION_TYPE" = "wayland" ]; then
            wl-paste -n | tcb-push
        else
            xclip -selection clipboard -o 2>/dev/null | tcb-push
        fi
        echo "Current clipboard contents synchronized manually."
        ;;
    *)
        show_help
        ;;
esac