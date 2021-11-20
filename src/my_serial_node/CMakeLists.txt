cmake_minimum_required(VERSION 2.8.3)
project( my_serial_node )

find_package(catkin REQUIRED COMPONENTS
  roscpp
  serial
  std_msgs
  tf
  nav_msgs
  decision
)

catkin_package(
  CATKIN_DEPENDS
    serial
    std_msgs
    decision
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_executable( my_serial_node src/my_serial_node.cpp)

target_link_libraries( my_serial_node
  ${catkin_LIBRARIES}
)

