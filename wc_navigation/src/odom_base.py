#!/usr/bin/env python

import math
from math import sin, cos, pi
import rospy
import tf
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Point, Pose, Quaternion, Twist, Vector3

rospy.init_node('odometry_publisher')

odom_pub = rospy.Publisher("odom", Odometry, queue_size=50)
odom_broadcaster = tf.TransformBroadcaster()

current_time = rospy.Time.now()
last_time = rospy.Time.now()

def callback(data):
    global odom
    odom = Odometry()
    odom.header.stamp = current_time
    odom.header.frame_id = "odom"

     # set the position
    odom.pose.pose = Pose(Point(x, y, 0.), Quaternion(*odom_quat))

     # set the velocity
    odom.child_frame_id = "base_link"
    odom.twist.twist = Twist(Vector3(vx, vy, 0), Vector3(0, 0, vth))
r = rospy.Rate(1.0)
while not rospy.is_shutdown():
    current_time = rospy.Time.now()
    rospy.Subscriber("odom_rf2o", Odometry, callback)

     # first, we'll publish the transform over tf
    odom_broadcaster.sendTransform((x, y, 0.),
        odom_quat,
        current_time,
        "base_link",
        "odom"
     )

     # next, we'll publish the odometry message over ROS


     # publish the message
    odom_pub.publish(odom)

    last_time = current_time
    r.sleep()
