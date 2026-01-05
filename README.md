# Research Platform 6WD Rover

The projects main goal is to develop a six-wheeled research rover with independent wheel control and flexible steering. It serves as a platform for exploring robotics concepts such as Navigation and combines complex system level integration of ROS2 integration, navigation, sensoric, sensor fusion, autonomous navigation, and AI-assisted perception. The rover combines affordable hardware with modular software to experiment with complex system-level integration in a compact, flexible platform....

## System Overview

### Main System

- Raspberry Pi 5, 16GB
- Raspberry Pi AI Kit, Hailo-8L
- Intel SSD 530 Seriesl, 240GB
- Apacer T6B400, 4 GB SATA flash module (as dev repo)
- Intel RealSense Depth Camera 435i

#### Software

- Debian 12 (Bookworm) ARM64

#### Container

| Name | Function | Version | Other|
|-----------|--------------|----------|----------|
|ROS Master|main nodes| ROS2 Humble |----------|
|ROS Slave|--------------|ROS1 Melodic|----------|
|Bridge|ROS1 to ROS2 Bridge|ROS2 Galactic / ROS1 Noetic|----------|
|Sensors|-|ROS2 Humble, microROS|----------|
|SLAM|-|ROS2 Humble|----------|
|Navigation|-|ROS2 Humble|----------|
|Drive|-|ROS2 Humble, microROS (deactivated - future use)|----------|
|Telemetry|-|ROS2 Humble|----------|
|Gateway|-|ROS2 Humble|Network Bridge, VPN Connector|
|Discovery Server|-|ROS2 Humble|----------|
|Update|-|watchtower| or switcht to Kubernetes?|


### Sub-Systems

- Arduino DUE
- Sensor Hat: IMU, GPS, Lidar, 
- Micro ROS
- 





## Description

## Installation

see installation.md

## Usage

## Roadmap

## Contributing

```bash
cd existing_repo
git remote add origin http://gitlab.internal.dombrowsky.online/michael/research-platform-6wd-rover.git
git branch -M main
git push -uf origin main
```

## Project status
