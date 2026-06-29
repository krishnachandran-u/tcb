
# tcb - terminal clipboard

Blazing-fast, keyboard-driven terminal overlay to search and recall past clips.

## Build

### Install dependencies

Check your display protocol: 

```bash
echo $XDG_SESSION_TYPE
```

Currently tested properly only for X11 Ubuntu 22.04

### For X11

Debian/Ubuntu:

```bash
sudo apt update
sudo apt install xclip libx11-dev libxfixes-dev
sudo apt install libsqlite3-dev
```

Fedora / RHEL / CentOS Stream / Rocky Linux / AlmaLinux:

```bash
sudo dnf install xclip libX11-devel libXfixes-devel
```

Arch Linux / Manjaro / EndeavourOS: 

```bash
sudo pacman -S xclip libx11 libxfixes
```

openSUSE:

```bash
sudo zypper install xclip libX11-devel libXfixes-devel
```

Alpine Linux:

```bash
sudo apk add xclip libx11-dev libxfixes-dev
```

### For Wayland

Debian/Ubuntu:

```bash
sudo apt update
sudo apt install wl-clipboard
```

Fedora / RHEL / CentOS Stream / Rocky Linux / AlmaLinux:

```bash
sudo dnf install wl-clipboard
```

Arch Linux / Manjaro / EndeavourOS:

```bash
sudo pacman -S wl-clipboard
```

openSUSE:

```bash
sudo zypper install wl-clipboard
```

Alpine Linux:

```bash
sudo apk add wl-clipboard
```

### Install 

```bash
git clone git@github.com:krishnachandran-u/tcb.git
cd tcb
chmod +x install.sh
./install.sh
```

### Run

```bash
tcb
```

<!-- ```bash
tcb-daemon
``` -->

<!-- Or without installing:

```bash
./build/tcb-daemon
``` -->

## Debug 

Directory structure:

```bash
core.h
└── all module headers

tcb.h
├── core.h
└── all module headers

*.c
└── tcb.h
```

Check that the tcb-daemon is running:

```bash
ps -eaf | grep tcb-daemon
```

See tcb-daemon logs:

```bash
cat /tmp/tcb.log
```

See storage:
```bash
cat /tmp/tcb.db
```

Check that the socket file was actually generated: 

```bash
ls -la /tmp/tcb.sock
# Output should show the socket descriptor file exists!
```

Fire your client data payload pipeline string down the wire:

```bash
echo "If you don't answer the Tung Tung Tung Sahur call three times, you might just get a terrifying visit." | ./build/tcb-push
```

Read the indexed items right out of your in-memory cache over the IPC loop:

```bash
./build/tcb-pull
```

## Third-party software

This project includes the `clipnotify` utility by Christopher Down.

Original repository:
https://github.com/cdown/clipnotify

The original `clipnotify` project is included in the `third_party/clipnotify` directory. See its `LICENSE` file for licensing information.