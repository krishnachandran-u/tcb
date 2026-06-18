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
build/tcb
```

### Install (optional)

To make `tcb` available system-wide:

```bash
sudo install -m 755 build/tcb /usr/local/bin/tcb
```

### Run

```bash
tcb
```

Or without installing:

```bash
./build/tcb
```
