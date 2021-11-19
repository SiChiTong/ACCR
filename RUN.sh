gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; roscore; exec bash"

echo "Thanks for using ACCR Robotic Operation System. Please select the node(s) you want to launch (e.g. 13 for running communication node and decision node.):"
echo "1. Serial Node (Must connect to the robot);"
echo "2. Navigation Node;"
echo "3. Decision Node;"
echo "4. Simulation Environment"
read x



if [[ $x =~ ['1'] ]]; then
    gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; rosrun my_serial_node my_serial_node;exec bash"
    echo "Serial Node Ready"
fi

if [[ $x =~ ['3'] ]]; then
    gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; rosrun cleaning cleaning_node;exec bash"
    echo "Decision Node Ready"
fi

if [[ $x =~ ['4'] ]]; then
    gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; roslaunch playground_gazebo playground.launch;exec bash"
    echo "Simulation Ready"
fi

if [[ $x =~ ['2'] ]]; then
    gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; roslaunch nav_config bot_nav.launch;exec bash"
    echo "Navigation Node Ready"
fi

