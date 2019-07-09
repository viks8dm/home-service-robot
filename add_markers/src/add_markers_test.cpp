#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  // ros::Subscriber marker_loc_sub = n.subscribe("odom", 1, );

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  // counter
  uint32_t marker_counter = 0;

  while (ros::ok())
  {
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
    marker.scale.x = 1.0;
    marker.scale.y = 1.0;
    marker.scale.z = 1.0;

    // Set the color -- be sure to set alpha to something non-zero!
    marker.color.r = 0.0f;
    marker.color.g = 1.0f;
    marker.color.b = 0.0f;
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration();

    // publish pick-up marker, pause, hide, pause, publish drop-off marker, done
    switch (marker_counter) {
      case 0: // publish pick-up marker
        {
          ROS_INFO("publishing PICK-UP marker");
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
          // publish marker
          marker_pub.publish(marker);

          // increment marker_counter
          marker_counter++;
          break;
        }

        case 1:   // pause for 5 seconds
        {
          sleep(5);
          ROS_INFO("hiding PICK-UP marker");
          marker.action = visualization_msgs::Marker::DELETE;

          // increment marker_counter
          marker_counter++;
          break;
        }

        case 2:   // publish drop-off marker
        {
          sleep(5);
          ROS_INFO("adding drop-off marker");
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
          // publish marker
          marker_pub.publish(marker);

          // increment marker_counter
          marker_counter++;
          break;
        }

        case 3:   // wrap-up
        {
          ROS_INFO("DONEEEEEEEEEE");
          break;
        }

        default:
          ROS_INFO("ERROR: add-marker needs fix");
    }

    r.sleep();
  }
}
