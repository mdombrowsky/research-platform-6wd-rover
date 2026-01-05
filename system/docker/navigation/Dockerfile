# docker/Dockerfile.navigation.runtime
ARG BUILDER_TAG

FROM rover/builder:${BUILDER_TAG}

FROM ros:humble-ros-core

COPY --from=rover/builder:${BUILDER_TAG} /ws/install /opt/rover_ws

ENV ROS_DISTRO=humble

CMD ["bash", "-c", "\
  source /opt/ros/humble/setup.bash && \
  source /opt/rover_ws/setup.bash && \
  ros2 launch navigation bringup.launch.py"]
