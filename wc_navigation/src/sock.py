#!/usr/bin/env python
import rospy
from std_msgs.msg import String
import socket

def talker():
    pub = rospy.Publisher('biogoal', String, queue_size=10)
    rospy.init_node('goal_letter', anonymous=True)
    rate = rospy.Rate(10) 
    s = socket.socket()  
    port = 4444
    s.bind(("0.0.0.0", port)) #192.168.203.73
    
    while not rospy.is_shutdown():
        s.listen(5)
        c, addr = s.accept()
        print("data recieved")
        hello_str = c.recvfrom(1024)[0].decode('utf-8')
        rospy.loginfo(hello_str)
        pub.publish(hello_str)
        rate.sleep()
    s.close()
if __name__ == '__main__':
    try:
        talker()
    except rospy.ROSInterruptException:
        pass

