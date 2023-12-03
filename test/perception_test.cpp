#include <gtest/gtest.h>
#include <stdlib.h>
#include <rclcpp/rclcpp.hpp>
#include <tf2/LinearMath/Matrix3x3.h>

#include "std_msgs/msg/string.hpp"

#include "../include/Perception.hpp"

class TaskPerception : public testing::Test {
 public:
  rclcpp::Node::SharedPtr node_;
  cv::Mat m_img_feed;
  sensor_msgs::msg::LaserScan m_lidar_feed;
  rclcpp::NodeOptions options;
  image_transport::Subscriber sub;
  rclcpp::Node::SharedPtr img_node;
  rclcpp::Publisher<TWIST>::SharedPtr m_pub_vel;
  rclcpp::Node::SharedPtr percep_odom_node;
  rclcpp::Subscription<ODOM>::SharedPtr odom_sub;
  bool r_rotate_flag;
  bool l_rotate_flag;
  bool move_forward;
  bool stop_flag;
  bool next_location;
  double present_yaw;
  double initial_yaw;
  void odom_callback_search(const ODOM::SharedPtr msg);
};

void TaskPerception::odom_callback_search(const ODOM::SharedPtr msg) {
    present_yaw = msg->pose.pose.orientation.y;
}


TEST_F(TaskPerception, test_detect_bin) {
  img_node = rclcpp::Node::make_shared("image_listener", options);
  m_pub_vel = img_node->create_publisher<TWIST>("cmd_vel", 10);
  percep_odom_node = rclcpp::Node::make_shared("percep_odom_node");
  odom_sub = percep_odom_node->create_subscription<ODOM>("odom", 10,
      std::bind(&TaskPerception::odom_callback_search, this, _1));
  node_ = rclcpp::Node::make_shared("test_publisher");
  auto test_pub = node_->create_publisher<std_msgs::msg::String>
                    ("perception", 10.0);

  auto num_pub = node_->count_publishers("perception");
  EXPECT_EQ(1, static_cast<int>(num_pub));

  rclcpp::spin_some(percep_odom_node);
    initial_yaw = present_yaw;
    r_rotate_flag = false;
    l_rotate_flag = false;
    move_forward = false;
    stop_flag = false;
    next_location = false;

    image_transport::ImageTransport it(img_node);
    EXPECT_FALSE(r_rotate_flag);
}

TEST_F(TaskPerception, test_move_bin) {
  img_node = rclcpp::Node::make_shared("image_listener", options);
  m_pub_vel = img_node->create_publisher<TWIST>("cmd_vel", 10);
  percep_odom_node = rclcpp::Node::make_shared("percep_odom_node");
  odom_sub = percep_odom_node->create_subscription<ODOM>("odom", 10,
      std::bind(&TaskPerception::odom_callback_search, this, _1));
  node_ = rclcpp::Node::make_shared("test_publisher");
  auto test_pub = node_->create_publisher<std_msgs::msg::String>
                    ("perception", 10.0);

  auto num_pub = node_->count_publishers("perception");
  EXPECT_EQ(1, static_cast<int>(num_pub));

  rclcpp::spin_some(percep_odom_node);
    initial_yaw = present_yaw;
    r_rotate_flag = false;
    l_rotate_flag = false;
    move_forward = false;
    stop_flag = false;
    next_location = false;

    auto vel = TWIST();
    if (r_rotate_flag) {
        vel.angular.z = -0.1;
        vel.linear.x = 0;
    }
    m_pub_vel->publish(vel);

    image_transport::ImageTransport it(img_node);
    // Start detecting the bin
    EXPECT_FALSE(stop_flag);
}
