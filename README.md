# MANTIS (Mapping and Navigation Technology Integrating SLAM)
MANTIS, short for Mapping and Navigation Technology Integrating SLAM, currently relies on Hector Mapping to facilitate its mapping operations. This approach enables MANTIS to generate detailed maps of its surroundings while simultaneously navigating through them. However, in the ongoing pursuit of enhanced functionality and performance, there are imminent plans to integrate Gmapping into the MANTIS system.
By incorporating Gmapping, MANTIS aims to further refine its mapping capabilities, allowing for more accurate and efficient navigation in diverse environments.



https://github.com/Avisheet/Mantis/assets/59338120/e65eb5c8-eb00-4cac-90d0-b5668eacf21c






## File Structure
- **joy_py**: Joy_py facilitates the control of a SLAM bot via an Xbox controller.
- **Low Level Codes**: It consist of the code for Arduino Mega.
- **rplidar_ros**: Standard package for getting sensor data from RPLiDAR.
- **wc_navigation**: Relevant launch files for navigation of MANTS in 2D mode.

## Pre-requisits:
Before deploying MANTIS, it's crucial to verify that all necessary libraries are installed both in the Arduino IDE and on your device.
- **Setting Up the Device.**
```
# In terminal write the following commands:
sudo apt install ros-noetic-rosserial
sudo apt install ros-noetic-rosserial arduino
sudo apt install ros-noetic-rosserial-python
```
- **After this you need to install rosserial in your Arduino_Ide**

## Launching navigation

### 2D Mapping

For mapping the arena using Hector SLAM

**Launch rplidar node**

```
roslaunch rplidar_ros rplidar.launch
```

**Launching the mapping node** 

```
roslaunch wc_navigation map_hector.launch)
```

**Saving the map**

```
rosrun map_server map_saver -f test_1
```

---

### 2D Navigation

To launch the navigation using only RViZ 

```
roslaunch wc_navigation wc_navigation.launch
```
## **_[Team ROBOSOCROS](https://github.com/RobosocNITH-ROS)_**
