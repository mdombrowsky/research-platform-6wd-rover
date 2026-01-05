# Dockerfile.development
# Basic ROS2 Humble Base template

FROM ros:humble-ros-base


# install common dev tools
RUN apt-get update \
 && apt-get install -y --no-install-recommends \
    vim \
    less \
    curl \
    git \
    emacs \
    mc \
    build-essential \
    python3-pip \
    usbutils \
    iproute2 \
    iputils-ping \
    ccache \
    ros-humble-rclcpp \
    ros-humble-rclpy \
    ros-humble-ament-cmake \
    ros-humble-std-msgs \
    ros-humble-example-interfaces \
    ros-humble-demo-nodes-py \
    ros-humble-demo-nodes-cpp \
    ros-humble-rosidl-typesupport-c \
    python3-ament-package \
    python3-colcon-common-extensions \
    python3-rosdep \
 && rm -rf /var/lib/apt/lists/*

 # also install:
   #openssh-server

# Set working directory
WORKDIR /workspace

#COPY ros_entrypoint.sh /ros_entrypoint.sh
#RUN chmod +x /ros_entrypoint.sh
#ENTRYPOINT ["/ros_entrypoint.sh"]

# Environment
ENV ROS_DISTRO=humble

# ccache for faster C++ builds
ENV PATH="/usr/lib/ccache:${PATH}"

# Default command
#CMD ["tail", "-f", "/dev/null"]
CMD ["bash"]
