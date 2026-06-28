#!/usr/bin/env bash

show_help() {
    echo "Usage: tcb [start|stop|restart|status|logs]"
    echo ""
    echo "Commands:"
    echo "  start     Start the tcb daemon and clipboard watch"
    echo "  stop      Stop the tcb service"
    echo "  restart   Restart the tcb service"
    echo "  status    Check if tcb is currently running"
    echo "  logs      View real-time logs for tcb"
    exit 1
}

if [ -z "$1" ]; then
    show_help
fi

case "$1" in
    start)
        systemctl --user start tcb
        echo "tcb started."
        ;;
    stop)
        systemctl --user stop tcb
        echo "tcb stopped."
        ;;
    restart)
        systemctl --user restart tcb
        echo "tcb restarted."
        ;;
    status)
        systemctl --user status tcb
        ;;
    logs)
        journalctl --user -u tcb -f
        ;;
    *)
        show_help
        ;;
esac