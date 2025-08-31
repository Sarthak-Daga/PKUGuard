# PKU-Guard (dev repo)

Goal: implement and test PKU-Guard (Memory isolation using Intel Protection Keys).
We develop on an AMD laptop and run tests inside QEMU emulating an Intel CPU with PKU.

Repo layout (initial):
- scripts/         # helper scripts (setup, qemu launcher)
- userland/        # small userland tests (pkey_alloc, pkey_mprotect demo)
- kernel/          # kernel-related work / patches (later)
- docs/            # notes, design doc, benchmark plan

Tonight:
- Add `userland/pku-demo.c` (userland demo) and `Makefile`.
- Add `scripts/run_qemu.sh` (QEMU template).
- Add `scripts/setup_dev.sh` to install build/qemu tools (for Debian/Ubuntu).

See `userland/README.md` for how to compile/run the demo (in a guest with PKU enabled).
