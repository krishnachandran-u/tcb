
# tcb - terminal clipboard

Blazing-fast, keyboard-driven terminal overlay to search and recall past clips.

## Build

### Clone the repository

```bash
git clone git@github.com:krishnachandran-u/tcb.git
cd tcb
```

### Build

```bash
cmake -S . -B build && cmake --build build
```

The resulting executable will be:

```text
build/tcb-daemon
```

### Install (optional)

To make `tcb` available system-wide:

```bash
sudo install -m 755 build/tcb-daemon /usr/local/bin/tcb-daemon
```

### Run

```bash
tcb-daemon
```

Or without installing:

```bash
./build/tcb-daemon
```

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