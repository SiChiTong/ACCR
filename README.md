# UoM_BT_Robot

This project is developed based on **[Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/)** and **[ROS Noetic](http://wiki.ros.org/noetic)**.

# Instructions:
## 1. ROS Installation:
ROS installation instructions can be found [here](http://wiki.ros.org/noetic/Installation/Ubuntu). We recommend the *ros-noetic-desktop-full*.

## 2. Packages Installation:
Install necessary dependency packages (some of them may not be required now):

    $ source /opt/ros/noetic/setup.bash
    $ sudo apt-get install -y ros-noetic-joy ros-noetic-teleop-twist-joy ros-noetic-teleop-twist-keyboard ros-noetic-laser-proc ros-noetic-rgbd-launch ros-noetic-depthimage-to-laserscan ros-noetic-rosserial-arduino ros-noetic-rosserial-python ros-noetic-rosserial-server ros-noetic-rosserial-client ros-noetic-rosserial-msgs ros-noetic-amcl ros-noetic-map-server ros-noetic-move-base ros-noetic-urdf ros-noetic-xacro  ros-noetic-compressed-image-transport ros-noetic-rqt-image-view ros-noetic-gmapping ros-noetic-interactive-markers ros-noetic-arbotix ros-noetic-geometry2 ros-noetic-joint-state-controller ros-noetic-effort-controllers ros-noetic-position-controllers ros-noetic-ddynamic-reconfigure ros-$ROS_DISTRO-realsense2-camera python3-catkin-pkg
    $ sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
    $ sudo apt install catkin
    $ sudo apt update && sudo apt upgrade

## 3. catkin_make
As long as the package is under some child directory of src, catkin can detect it automatically. Next step is then to build the system.

    $ source /opt/ros/noetic/setup.bash
    $ cd <yourpath>/UoM_BT_Robot
    $ catkin_make

## 4. Save the test site to the simulation (no longer needs)

    $ cd <yourpath>/UoM_BT_Robot
    $ source devel/setup.bash
    $ roslaunch test_site_gazebo gazebo.launch
    
Once the Gazebo is running, on your left hand, find and click `<Models>` - `<test_site_gazebo>`. At bottom, click `<pose>`, change `<x>`,`<y>`,`<z>`,`<pitch>`,`<yaw>` to 0. But keep `<roll>` value as whatever it is. After you complete these steps, save the world to 
The Gazebo should be opened then.  Then choose "pose" below. Change "x,y,z,pitch,yaw"values to 0, BUT KEEP ROLL VALUE as whatever it is. After you done all of this, save the world to

    $ <yourpath>/UoM_BT_Robot/src/mbot_gazebo/mbot_simulations/worlds/test_site_gazebo.world
    
## 5. Register your Path and shell
    
    $ cd <yourpath>/UoM_BT_Robot
    $ sudo chmod +x RUN.sh
    $ cd <yourpath>/UoM_BT_Robot/src/mbot_gazebo/mbot/mbot_description/scripts/
    $ sudo chmod +x demo_path.py
    
## 6. Test your project

    $ cd <yourpath>/UoM_BT_Robot
    $ ./RUN.sh


# TODO
* The current version mixed up the simulation and the algorithms, will update a new version later
* All algorithms need further improvement
* The physical model has some setup problem in the simulation 

