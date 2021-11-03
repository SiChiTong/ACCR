gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; roslaunch mbot_description mbot_slam.launch;exec bash"
sleep 3
gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; roslaunch mbot_navigation mbot_navigation.launch;exec bash"
sleep 3
# gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; rosrun my_serial_node my_serial_node;exec bash"
sleep 5
echo "If you want to run the pre-defined path, please input: T, then press enter. Else: press anykey and enter."
read x
if [ $x == "T" ]
then
	gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; rostopic echo cmd_vel;exec bash"
	#source /opt/ros/noetic/setup.bash
	#source devel/setup.bash 
	#defined_PATH=./src/mbot_gazebo/mbot/mbot_description/scripts/
	#cd "${defined_PATH}"
	#python3 demo_path.py
	gnome-terminal -- bash -c "source /opt/ros/noetic/setup.bash; source devel/setup.bash; rosrun decision demo_path.py;exec bash"	
else
	echo "Enjoy :) "
fi
