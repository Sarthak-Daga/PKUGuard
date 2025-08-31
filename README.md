# PKUGuard

Memory isolation via Intel PKU (Protection Keys for Userspace)  
Course Project - Operating Systems

## Structure
- `scripts/` → setup & automation scripts
- `userland/` → demo programs + Makefile
- `ToDo` → quick commands / setup notes

## Quickstart
```bash
chmod +x scripts/*.sh
make -C userland
./userland/pku-demo

