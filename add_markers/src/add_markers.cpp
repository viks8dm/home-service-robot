#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
// #include <nav_msgs/Odometry.h>
#include <std_msgs/UInt8.h>

// bool marker_reach_state = false;
uint8_t goal_reach_state = 0;

/* robot goal proximity callback function */
void goalReachCallback(const std_msgs::UInt8::ConstPtr& msg)
{
   goal_reach_state = msg->data;
   return;
}

/* main function */
int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");

  ros::NodeHandle n;
  ros::Rate r(5);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  ros::Subscriber odom_sub = n.subscribe("/goal_reached", 1, goalReachCallback);
  bool done = false;

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  ROS_INFO("Subscribed to desired goal-position");

  while (ros::ok()) {
    //Do this every cycle to ensure the subscriber receives the message
    ros::spinOnce();
    visualization_msgs::Marker marker;
    // Set the frame ID and timestamp.  See the TF tutorials for information on these.
    marker.header.frame_id = "/map";
    marker.header.stamp = ros::Time::now();

    // Set the namespace and id for this marker.  This serves to create a unique ID
    // Any marker sent with the same namespace and id will overwrite the old one
    marker.ns = "basic_shapes";
    marker.id = 0;
    // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
    marker.type = shape;
    // Set the scale of the marker -- 1x1x1 here means 1m on a side
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;
    // threshold = marker.scale.x;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    switch (goal_reach_state)
    {
      case 0: // publish pick-up marker
        {
          //ROS_INFO("publishing PICK-UP marker");
          // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
          marker.action = visualization_msgs::Marker::ADD;
          // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
          n.getParam("/pick_up_loc/tx", marker.pose.position.x);
          n.getParam("/pick_up_loc/ty", marker.pose.position.y);
          n.getParam("/pick_up_loc/tz", marker.pose.position.z);
          n.getParam("/pick_up_loc/qx", marker.pose.orientation.x);
          n.getParam("/pick_up_loc/qy", marker.pose.orientation.y);
          n.getParam("/pick_up_loc/qz", marker.pose.orientation.z);
          n.getParam("/pick_up_loc/qw", marker.pose.orientation.w);
          break;
        } // case 0

        case 1:   // robot reach pickup site, delete pick-up marker
          {
            sleep(2);
            //ROS_INFO("hiding PICK-UP marker");
            marker.action = visualization_msgs::Marker::DELETE;
            break;
          } // case 1

        case 2: // wait for robot to reach drop-off site
          {
            marker.action = visualization_msgs::Marker::DELETE;
            break;
          }

        case 3:   // publish drop-off marker
          {
            sleep(5);
            //ROS_INFO("adding drop-off marker");
            // Set the marker action.  Options are ADD, DELETE, and new in ROS Indigo: 3 (DELETEALL)
            marker.action = visualization_msgs::Marker::ADD;
            // Set the pose of the marker.  This is a full 6DOF pose relative to the frame/time specified in the header
            n.getParam("/drop_off_loc/tx", marker.pose.position.x);
            n.getParam("/drop_off_loc/ty", marker.pose.position.y);
            n.getParam("/drop_off_loc/tz", marker.pose.position.z);
            n.getParam("/drop_off_loc/qx", marker.pose.orientation.x);
            n.getParam("/drop_off_loc/qy", marker.pose.orientation.y);
            n.getParam("/drop_off_loc/qz", marker.pose.orientation.z);
            n.getParam("/drop_off_loc/qw", marker.pose.orientation.w);
            done = true;
            break;
          }

    } // switch

    // Publish the marker
    while (marker_pub.getNumSubscribers() < 1)
    {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
    }

    //publish the marker
    marker_pub.publish(marker);

    // if last marker published and noted as done exit
    if (done) {
      ROS_INFO("=== DESTINATION Reached ===");
      sleep(7);
      return 0;
      }

    r.sleep();
  } // while ros-ok

  return 0;
} // main
