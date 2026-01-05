# Install on RPI

Raspberry Pi 3 Model B Rev 1.2
4 CPU cores, ARM Cortex-A53 (0xd03), 1GB RAM

OS version: Debian 12 Bookworm
Patch level: 12.7
Kernel: 6.6.51+rpt-rpi-v8, built 2024-10-08

Raspberry Pi reference 2024-10-22
Generated using pi-gen, https://github.com/RPi-Distro/pi-gen, ecda2579ebbc125b321984c571e3128f6fd959d4, stage2

## Flash SD Card

Image used for RPI3B: raspios_arm64-2024-10-28  
https://downloads.raspberrypi.com/raspios_arm64/images/raspios_arm64-2024-10-28/2024-10-22-raspios-bookworm-arm64.img.xz  
Unpack and flash with balenaEtcher.

## Prepare Development Flash Drive

```bash
sudo wipefs -a /dev/sda1
sudo mkfs.xfs -f /dev/sda1
sudo mkdir -p /mnt/repo
sudo mount -o defaults /dev/sda1 /mnt/repo
sudo chown -R michael:michael /mnt/repo
sudo chmod -R 755 /mnt/repo
```

## Clone Repo on Flash Drive or onto SD Card

clone repo as /mnt/repo/... (without "research-platform-6wd-rover")

```bash
cd /mnt/repo
git clone --branch draft --recurse-submodules http://gitlab.internal.dombrowsky.online/michael/research-platform-6wd-rover .
```

update submodules

```bash
git submodule update --init --recursive
```

run

```bash
./install.sh
```

