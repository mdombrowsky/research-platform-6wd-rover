# docker/Dockerfile.builderR2

FROM ros:humble-ros-base

WORKDIR /ws

RUN apt-get update && apt-get install -y \
    python3-colcon-common-extensions \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY workspace/src ./src

RUN . /opt/ros/humble/setup.sh && \
    colcon build --merge-install

# install space ends up in /ws/install
