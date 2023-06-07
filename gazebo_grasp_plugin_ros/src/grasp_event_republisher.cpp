#include <gazebo/transport/transport.hh>

#include "gazebo_grasp_plugin_ros/msgs/grasp_event.pb.h"
#include <gazebo_grasp_plugin_ros/msg/gazebo_grasp_event.hpp>
#include <rclcpp/rclcpp.hpp>

using GraspEventPtr = std::shared_ptr<const gazebo::msgs::GraspEvent>;

rclcpp::Publisher<gazebo_grasp_plugin_ros::msg::GazeboGraspEvent>::SharedPtr graspEventPublisher;

/**
 * Callback to receive gazebo grasp event messages
 */
void ReceiveGraspMsg(const GraspEventPtr& gzMsg)
{
  RCLCPP_INFO_STREAM(rclcpp::get_logger("gazebo_grasp_plugin_event_republisher"), 
                     "Re-publishing grasp event: " << gzMsg->DebugString());
  auto rosMsg = std::make_unique<gazebo_grasp_plugin_ros::msg::GazeboGraspEvent>();
  rosMsg->arm      = gzMsg->arm();
  rosMsg->object   = gzMsg->object();
  rosMsg->attached = gzMsg->attached();
  graspEventPublisher->publish(std::move(rosMsg));
}

int main(int argc, char** argv)
{
  // Initialize ROS and Gazebo transport
  rclcpp::init(argc, argv);
  if (!gazebo::transport::init())
  {
    RCLCPP_ERROR(rclcpp::get_logger("gazebo_grasp_plugin_event_republisher"), 
                 "Unable to initialize gazebo transport - is gazebo running?");
    return 1;
  }
  gazebo::transport::run();

  // Subscribe to Gazebo grasp event message
  gazebo::transport::NodePtr gzNode(new gazebo::transport::Node());
  gzNode->Init();
  gazebo::transport::SubscriberPtr subscriber;
  try
  {
    subscriber = gzNode->Subscribe("~/grasp_events", &ReceiveGraspMsg);
  }
  catch (std::exception& e)
  {
    RCLCPP_ERROR_STREAM(rclcpp::get_logger("gazebo_grasp_plugin_event_republisher"), 
                        "Error subscribing to topic: " << e.what());
    return 1;
  }

  // Initialize ROS publisher
  auto rosNode = rclcpp::Node::make_shared("gazebo_grasp_plugin_event_republisher");
  const std::string pubTopic = "grasp_events";
  graspEventPublisher =
    rosNode->create_publisher<gazebo_grasp_plugin_ros::msg::GazeboGraspEvent>(pubTopic, 10);

  rclcpp::spin(rosNode);
  rclcpp::shutdown();
  gazebo::transport::fini();
  return 0;
}
