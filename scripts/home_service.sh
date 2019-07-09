#!/bin/sh

# launch turtlebot_world.launch to deploy turtlebot environment
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
export ROBOT_INITIAL_POSE='-x -5 -y -2 -z 0 -R 0 -P 0 -Y 0';
roslaunch turtlebot_gazebo turtlebot_world.launch  world_file:=$(pwd)/../../src/map/home_service.world " &

sleep 15

# launch amcl_demo.launch for localization
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(pwd)/../../src/map/home_service_map.yaml " &

sleep 2

# launch rviz for visualization
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
roslaunch add_markers home_service_rviz_config.launch rviz_config_file:=$(pwd)/../rvizConfig/home_service.rviz" &

sleep 15 # keeping large to enable visualization

# launch add_markers node
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
rosparam load $(pwd)/../config/marker_config.yaml;
rosrun add_markers add_markers " &

sleep 2

# launch pick_objects node
xterm -e "cd $(pwd)/../..;
source devel/setup.bash;
rosparam load $(pwd)/../config/marker_config.yaml;
rosrun pick_objects pick_objects" &




