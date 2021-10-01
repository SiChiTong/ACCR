# ACCR
This project is developed by Human Robotics Lab at the University of Melbourne based on Ubuntu 20.04 Focal Fossa and ROS Noetic. If you have any questions, feel free to contact me: Murphy.xu@unimelb.edu.au
cd to devel: source setup.bash
stay in devel folder: roslaunch playground_gazebo playground.launch
Use: rostopic pub /cmd_vel geometry_msgs/Twist -- '[0.0, 0.0, 0.0]' '[0.0, 0.0, 0.0]' to control the robot
Use: rostopic echo /imu to read sensor readings.
The "playground_gazebo" folder contains the testing field
The "ugv_description" folder contains the urdf file of the vahicle.
*If one wants to update the speed of the vehicle, please copy and past the 4th line or type in the command, do not use the "up arrow".
