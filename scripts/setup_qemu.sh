qemu-system-x86_64 \
  -m 4G -smp 2 \
  -cpu Skylake-Client,+pku \
  -hda images/ubuntu.img \
  -cdrom images/ubuntu-24.04.3-live-server-amd64.iso \
  -boot d

