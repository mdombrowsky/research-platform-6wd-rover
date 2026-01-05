#!/bin/bash
LABEL="robot"
MOUNT_POINT="/mnt/hdd"

# Check if the disk with the specified label is available
DEVICE=$(blkid | grep "LABEL=\"$LABEL\"" | awk -F: '{print $1}')

if [ -z "$DEVICE" ]; then
    echo "Error: Disk with label $LABEL is not available" > /dev/ttyAMA0
else
    # Device is found, mount it
    echo "Disk with label $LABEL is available. Mounting $DEVICE to $MOUNT_POINT" > /dev/ttyAMA0
    sudo mkdir -p /mnt/hdd
    sudo mount "$DEVICE" "$MOUNT_POINT"

    # Check if the mount was successful
    if mount | grep -q "$MOUNT_POINT"; then
        echo "Successfully mounted $DEVICE to $MOUNT_POINT" > /dev/ttyAMA0
    else
        echo "Failed to mount $DEVICE" > /dev/ttyAMA0
    fi
fi

# if hdd mount was successful:
# start docker container
# create docker container from humble-base
#   container ros-dev
# mount  /mnt/hdd/ros2_ws:/ros2_ws
# use host network
# pass arduino on usb
# pass intel camera on usb



# docker run -v /mnt/hdd/ros2_ws:/ros2_ws --name humble_test ros:humble-ros-base


# container ros-live 
