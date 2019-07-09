#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

int main(int argc, char** argv){
  // Initialize the simple_navigation_goals node
  ros::init(argc, argv, "pick_objects");

  //tell the action client that we want to spin a thread by default
  MoveBaseClient ac("move_base", true);

  // Wait 5 sec for move_base action server to come up
  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }

  move_base_msgs::MoveBaseGoal goal;		// pick-up & drop goal

  // set up the frame parameters
  // pick-goal setup
  goal.target_pose.header.frame_id = "map";
  goal.target_pose.header.stamp = ros::Time::now();

  ros::NodeHandle n; // node-handle for parameters

  // Define a position and orientation for the robot to reach
  n.getParam("/pick_up_loc/tx", goal.target_pose.pose.position.x);
  n.getParam("/pick_up_loc/ty", goal.target_pose.pose.position.y);
  n.getParam("/pick_up_loc/tz", goal.target_pose.pose.position.z);
  n.getParam("/pick_up_loc/qx", goal.target_pose.pose.orientation.x);
  n.getParam("/pick_up_loc/qy", goal.target_pose.pose.orientation.y);
  n.getParam("/pick_up_loc/qz", goal.target_pose.pose.orientation.z);
  n.getParam("/pick_up_loc/qw", goal.target_pose.pose.orientation.w);
  ac.sendGoal(goal);
  // Send the goal position and orientation for the robot for pick-up location
  ROS_INFO("Sending pick-up goal");
  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("YUUUHHHOOOOOOOOO, robot reached PICK-UP location");
    // wait a bit before next message
    ros::Duration(5.0).sleep();
  }
  else {
    ROS_INFO("The robot failed to reach pick-up location");
    return 0;
  }

  // Define a position and orientation for the robot to reach
  n.getParam("/drop_off_loc/tx", goal.target_pose.pose.position.x);
  n.getParam("/drop_off_loc/ty", goal.target_pose.pose.position.y);
  n.getParam("/drop_off_loc/tz", goal.target_pose.pose.position.z);
  n.getParam("/drop_off_loc/qx", goal.target_pose.pose.orientation.x);
  n.getParam("/drop_off_loc/qy", goal.target_pose.pose.orientation.y);
  n.getParam("/drop_off_loc/qz", goal.target_pose.pose.orientation.z);
  n.getParam("/drop_off_loc/qw", goal.target_pose.pose.orientation.w);

  // robot reached pick-up location, send drop-off location
  ROS_INFO("Sending drop-off goal");
  ac.sendGoal(goal);
  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its drop goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
    ROS_INFO("YUUUHHHOOOOOOOOO, robot reached DROP-OFF location");
  }
  else {
    ROS_INFO("The robot failed to reach drop-off location");
  }

  return 0;
}
