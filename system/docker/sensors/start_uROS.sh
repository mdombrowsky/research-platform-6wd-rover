#!/bin/bash

echo "[INFO] Starting micro-ROS Agent..."
source /opt/ros/humble/setup.bash
source /workspace/install/setup.bash
ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyACM0
