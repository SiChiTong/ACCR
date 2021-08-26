import getopt
import math
import struct
import sys
import time

import actionlib
import genpy
import numpy as np
import rospy
from geometry_msgs.msg import Twist, Point, PoseWithCovarianceStamped
from move_base_msgs.msg import MoveBaseAction, MoveBaseGoal, MoveBaseResult, MoveBaseFeedback
from nav_msgs.msg import Odometry
from sensor_msgs.msg import LaserScan
from tf import transformations


# parameters
tolerence_position = 0.4
tolerence_orientation = 0.05

# publishers
pub = None
# Goal
class Controller:
    figureLine = [ \
        [2.0, -2.0, 0.0, 0], \
        [2.0, -2.0, 1.57, 1], \
        [2.0, 1.3, 1.57, 0], \
        [2.0, 1.3, 0.0, 1], \
        [3.1, 1.3, 0.0, 0], \
        [3.1, 1.3, -1.57, 1], \
        [3.1, -2.0, -1.57, 0], \
        [3.1, -2.0, 0.0, 1], \
        [4.9, -2.0, 0.0, 0], \
        [4.9, -2.0, 1.57, 1], \
        [4.9, 2.0, 1.57, 0], \
        [4.9, 2.0, 0.0, 1], \
        [6.6, 2.0, 0.0, 0], \
        [6.6, 2.0, -1.57, 1], \
        [6.6, -2.0, -1.57, 0], \
        [6.6, -2.0, 3.14, 1], \
        [1.0, -3.0, 3.14, 0], \
        ]
    
    # Global Variables
    PENDING = 0
    ACTIVE = 1
    DONE = 2
    WARN = 3
    ERROR = 4

    # Subscriber
    current_pose = None

    # robot state variables
    position = Odometry()

    # machine state
    state = [0, 1, 2, 3, 4, 5, 6]
    
    # callbacks
    def clbk_odom(self, msg):
        global position

        # position
        rate = rospy.Rate(10)
        # position = msg
        # rate.sleep()
        # print ('message')
        # print (msg.pose.pose)
        # rospy.loginfo("The current odom value: " + str(msg.pose.pose))

    def goal_set(self, x, y, yaw):
        global PENDING, ACTIVE, DONE, WARN, ERROR
        goal = MoveBaseGoal()
        goal.target_pose.header.frame_id = "map"
        goal.target_pose.header.stamp = rospy.Time.now()
        goal.target_pose.pose.position.x = x
        goal.target_pose.pose.position.y = y

        # to send orientation with a yaw we need quaternion transform
        x , y, z, w = transformations.quaternion_from_euler(0, 0, yaw)
        goal.target_pose.pose.orientation.x = x
        goal.target_pose.pose.orientation.y = y
        goal.target_pose.pose.orientation.z = z
        goal.target_pose.pose.orientation.w = w
        now = rospy.get_rostime()
        print('[%i.%i] PubMove: x,y,yaw of %f %f %f' % \
              (now.secs, now.nsecs, x, y, yaw))

        action_server_name = '/move_base'
        move_base_client = actionlib.SimpleActionClient('/move_base', MoveBaseAction)
        rospy.loginfo('Waiting for action Server ' + action_server_name)
        move_base_client.wait_for_server()
        rospy.loginfo('Action Server Found...' + action_server_name)

        # pub = rospy.Publisher('/cmd_vel', Twist, queue_size=1)

        # Send Goal
        move_base_client.send_goal(goal)
        state_result = move_base_client.get_state()
        now = rospy.get_rostime()

        print('[%i.%i] Waiting for result ...' % (now.secs, now.nsecs))
        # wait = move_base_client.wait_for_result()
        # wait = client.wait_for_result()
        while state_result < self.DONE:
        # while not wait:
            # rospy.logerr("Action server not available!")
            # rospy.signal_shutdown("Action server not available!")
            now = rospy.get_rostime()
            state_result = move_base_client.get_state()
            print("state_result: "+str(state_result))
            return state_result

        rospy.loginfo("[Result] State: " + str(state_result))
        if state_result == ERROR:
            rospy.logerr("Something went wrong in the Server Side")
        if state_result == WARN:
            rospy.logwarn("There is a warning in the Server Side")


    def compare(self, waypoint, position, orientation_check):
        global tolerence_position, tolerence_orientation
        
        print ('Compare')
        print (position.pose.pose)
        result = 0
        p1 = np.array([position.pose.pose.position.x, position.pose.pose.position.y])
        p2 = np.array([waypoint[0], waypoint[1]])
        p3 = p2 - p1
        p4 = math.hypot(p3[0], p3[1])
        print ('P4 = ' + str(p4))
        
        if orientation_check == 0:
            if p4 <= tolerence_position:
                rospy.loginfo('Reach goal')
                result = 1
                return result
            else:
                rospy.loginfo('Position not match')
                result = 0
                return result
            
        elif orientation_check == 1:
            x , y, z, w = transformations.quaternion_from_euler(0, 0, waypoint[2])
            o1 = abs(position.pose.pose.orientation.z - z)
            if o1 <= tolerence_orientation:
                rospy.loginfo('Reach goal')
                result = 1
                return result
            else:
                rospy.loginfo('Orientation not match')
                result = 0
                return result

    def __init__(self):
        rospy.init_node('controller')

        # Time per loop for the main control
        self.loop_msec = 50


    def run(self):
        global pub
        global current_pose
        global state
        global position

        # waitAtEachVertex = 0 # Set this to 0 for continual movements
        # waypointName = 'Line'
        waypointList = self.figureLine

        current_pose = rospy.Subscriber('/odom', Odometry, self.clbk_odom)
        rate = rospy.Rate(5)
        while (True):
            s = 0
            for waypoint in waypointList:
                x, y, yaw, orientation_check= waypoint
                now = rospy.get_rostime()
                # publish the waypoint
                moveResult = self.goal_set(x, y, yaw)

                #print('[%i.%i] Waypoint: %f X %f Y %f yaw' % \
                #  (now.secs, now.nsecs, x, y, yaw))
                # current_pose = rospy.Subscriber('/odom', Odometry, self.clbk_odom)
                compare_result = 0
                rospy.loginfo("state_result: " + str(compare_result))
                i = 0
                while compare_result == 0:
                    rate = rospy.Rate(10)
                    #if i%5 == 0:
                    #   moveResult = self.goal_set(x, y, yaw)
                    
                    # current_pose = rospy.Subscriber('/odom', Odometry, self.clbk_odom)
                    var=rospy.wait_for_message('/odom',Odometry,timeout=5)
                    compare_result = self.compare(waypoint, var, orientation_check)
                    print('[%i.%i] Waypoint: %f X %f Y %f yaw' % \
                      (now.secs, now.nsecs, x, y, yaw))
                    rospy.loginfo("state_result: " + str(s) + "  Waypoint: " + str(waypoint))
                    # print('Not finish yet')
                    rate.sleep()
               
                # while state_result < DONE:

                # if state_result == ERROR:
                #   rospy.logerr("Something went wrong in the Server Side")
                # if state_result == WARN:
                #   rospy.logwarn("There is a warning in the Server Side")
                s =+ 1
            i=0
            while (True):
               pub = rospy.Publisher('/cmd_vel', Twist, queue_size=1) 
               Stop = Twist()
               Stop.linear.x = 0.0
               Stop.angular.z = 0.0
               pub.publish(Stop)
               rate.sleep()

if __name__ == "__main__":
    # Create an instance of our goal class
    node = Controller()
    # run it
    node.run()

