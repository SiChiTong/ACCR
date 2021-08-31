# Automatic Conveyor Cleaning Robot (ACCR)

This project is developed by Human Robotics Lab at the University of Melbourne based on **[Ubuntu 20.04 Focal Fossa](https://releases.ubuntu.com/20.04/)** (Kernel Version: 5.11.0-27-generic) and **[ROS Noetic](http://wiki.ros.org/noetic)**. If you have any questions, feel free to contact me: Murphy.xu@unimelb.edu.au

# Instructions:
## 1. ROS Installation
ROS installation instructions can be found [here](http://wiki.ros.org/noetic/Installation/Ubuntu). We recommend the *ros-noetic-desktop-full*. 

*_**NOTE:**_* As ROS has revoked the old GPG key, you may meet the following error during the ROS installation:

    Err:1 http://packages.ros.org/ros/ubuntu <YOUR_UBUNTU_VERSION>/main amd64 <SOME_ROS_PKG> amd64 0.13.3-0xenial-20190320-132757-0800
    
or,

    W: GPG error: http://packages.ros.org <YOUR_UBUNTU_VERSION> InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 5523BAEEB01FA116
    
To resolve this, you need to run the following code to remove the old key:
    
    $ sudo apt-key del 421C365BD9FF1F717815A3895523BAEEB01FA116
    
And then, import the new key:

    $ sudo -E apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654
    
At this point, make sure to run `$ sudo apt clean && sudo apt update`. You should receive no errors and subsequent `$ sudo apt install` commands should also now work.

## *Optional* (Recommanded)
You can use the following code to make your ROS environment working **globally**.

    $ echo "source /opt/ros/noetic/setup.bash" >> ~/.bashrc

In this case, in the following steps, you can always **ignore** the following code.

    $ source /opt/ros/noetic/setup.bash

This makes your life easier, as you don't need to include this line every time when you use a ROS command in a new terminal.

If you want to **remove** this global setting from your device, simply go to `Home`, and then press `Ctrl` + `H` to show all hidden files, then open the `.bashrc` file and delete the following line:

    source /opt/ros/noetic/setup.bash

## 2. Packages Installation
Install necessary dependency packages (some of them may be no longer required):

    $ source /opt/ros/noetic/setup.bash
    $ sudo apt-get install -y ros-noetic-joy ros-noetic-teleop-twist-joy ros-noetic-teleop-twist-keyboard ros-noetic-laser-proc ros-noetic-rgbd-launch ros-noetic-depthimage-to-laserscan ros-noetic-rosserial-arduino ros-noetic-rosserial-python ros-noetic-rosserial-server ros-noetic-rosserial-client ros-noetic-rosserial-msgs ros-noetic-amcl ros-noetic-map-server ros-noetic-move-base ros-noetic-urdf ros-noetic-xacro  ros-noetic-compressed-image-transport ros-noetic-rqt-image-view ros-noetic-gmapping ros-noetic-interactive-markers ros-noetic-arbotix ros-noetic-geometry2 ros-noetic-joint-state-controller ros-noetic-effort-controllers ros-noetic-position-controllers ros-noetic-ddynamic-reconfigure ros-$ROS_DISTRO-realsense2-camera python3-catkin-pkg
    $ sudo apt-get update && sudo apt-get upgrade && sudo apt-get dist-upgrade
    $ sudo apt install catkin
    $ sudo apt update && sudo apt upgrade

## 3. catkin_make
As long as the package is under some child directory of src, catkin can detect it automatically. The next step is then to build the system.

    $ source /opt/ros/noetic/setup.bash
    $ cd <yourpath>/ACCR
    $ catkin_make

## 4. Save the test site to the simulation

    $ cd <yourpath>/ACCR
    $ source devel/setup.bash
    $ roslaunch test_site_gazebo gazebo.launch
    
Once the Gazebo is running, on your left hand, find and click `<Models>` - `<test_site_gazebo>`. At the bottom, click `<pose>`, change `<x>`,`<y>`,`<z>`,`<pitch>`,`<yaw>` to 0. But keep `<roll>` value as whatever it is. After you complete these steps, save the world to 

    $ <yourpath>/ACCR/src/mbot_gazebo/mbot_simulations/worlds/test_site_gazebo.world
    
## 5. Register your Path and shell
    
    $ cd <yourpath>/ACCR
    $ sudo chmod +x RUN.sh
    $ cd <yourpath>/ACCR/src/mbot_gazebo/mbot/mbot_description/scripts/
    $ sudo chmod +x demo_path.py
    
## 6. Test your project

    $ cd <yourpath>/ACCR
    $ ./RUN.sh


# TODO
At this beginning level, we still have a lot of works that need to be done. Here are some short-term goals:

## *Code structure:*
* Isolate ROS controller from simulations, and allow users to choose to run both (to test in simulation) or the ROS controller only (to test on the robot).

## *Simulation:*
* Rebuild the map, and set the corner to the origin, which makes all coordinates in the map be positive numbers.
* Rewrite the robot description files to resolve some issues:
    * Enable the bucket joint and allow it to be lifted up or dropped down in the simulation environment. Currently, it is down to the bottom and potentially caused huge friction.
    * Modify the robot model, as it has only two wheels in the simulation, which made it has a pitch angle change. This pitch angle change may also cause huge friction when the acceleration changed (especially when slowing down).
* Potentially change or modify the *diff_robot_controller* to make it closer to the algorithms used in our real robot.

## *ROS controller*
* Clean up the algorithms used in the current project and remove or mark the irrelevant or unused packages.
* Create a simple decision layer to allow it to decide when to lift up or drop down the bucket (will add more features later: *e.g.* switch the navigation algorithms between lidar-base and vision-base).
* More features will come later.


