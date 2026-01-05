#!/bin/bash
set -euxo pipefail

# Usage: ./install.sh [dev|prod]
MODE="${1:-dev}"  # default

if [[ ! -f ./packages.list ]]; then
    echo "packages.list not found!" >&2
    exit 1
fi

source ./packages.list

if [[ "$MODE" == "dev" ]]; then
    PACKAGES=("${DEV_PACKAGES[@]}")
else
    PACKAGES=("${PROD_PACKAGES[@]}")
fi


if [[ "$MODE" == "dev" ]]; then
    echo "DEV MODE: updating and installing packages"
    # update and install packages
    sudo apt-get update
    sudo apt-get upgrade -y
    sudo apt-get install -y --no-install-recommends "${PACKAGES[@]}"
    # Optional: cleanup
    sudo apt-get autoremove -y
    sudo apt-get clean
fi

if [[ "$MODE" == "prod" ]]; then
    echo "PROD MODE: installing packages"
    sudo apt-get install --no-upgrade -y --no-install-recommends "${PACKAGES[@]}"
fi

if [[ ${#PACKAGES[@]} -eq 0 ]]; then
    echo "No packages to install for mode: $MODE"
    exit 0
fi

echo "Installing packages for mode: $MODE"
printf '%s\n' "${PACKAGES[@]}"

# setting up swap file
#sudo cp ../configs/dphys-swapfile /etc/dphys-swapfile
#sudo dphys-swapfile swapoff || true
#sudo dphys-swapfile setup
#sudo dphys-swapfile swapon

# add system scripts

# start system scripts in systemd

# pull docker images

# setup systemd services for docker containers


echo "System setup complete."