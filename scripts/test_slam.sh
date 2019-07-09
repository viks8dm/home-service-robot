#!/bin/sh

# launch turtlebot_world.launch to deploy turtlebot environment
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
export ROBOT_INITIAL_POSE='-x -5 -y -2 -z 0 -R 0 -P 0 -Y 0';
roslaunch turtlebot_gazebo turtlebot_world.launch  world_file:=$(pwd)/../../src/map/home_service.world " & 

sleep 15

# launch gmapping_demo.launch to perform slam_gmapping
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
rosparam set /slam_gmapping/iterations 10;
rosparam set /slam_gmapping/linearUpdate 0.05;
rosparam set /slam_gmapping/angularUpdate 0.1;
rosparam set /slam_gmapping/map_update_interval 0.25;
rosparam set /slam_gmapping/srr 0.01;
rosparam set /slam_gmapping/srt 0.01;
rosparam set /slam_gmapping/str 0.02;
rosparam set /slam_gmapping/stt 0.02;
roslaunch turtlebot_gazebo gmapping_demo.launch" &

sleep 5

# launch view_navigation for rviz
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
roslaunch turtlebot_rviz_launchers view_navigation.launch" &

sleep 10

# launch teleop_keyboard for manual control
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
roslaunch turtlebot_teleop keyboard_teleop.launch" &

# save using following on command line
# rosrun map_server map_saver -f <map-location-and-name>