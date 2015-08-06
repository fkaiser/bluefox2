#include "bluefox2/single_node.h"

namespace bluefox2 {

void SingleNode::Acquire() {
  // This is a hack of boosting frame rate, but only for 200wG
  // Need to find a way of doing this for all cameras
  if (boost_) {
    std::cout << "Boost mode!!!!!" << std::endl;
    bluefox2_ros_.RequestSingle();
    bluefox2_ros_.RequestSingle();
    bluefox2_ros_.RequestSingle();
    bluefox2_ros_.RequestSingle();
    bluefox2_ros_.RequestSingle();
  }

  while (is_acquire() && ros::ok()) {

	  // Request an image, i.e. put Request Object in Request queue if a Request Object is available
    bluefox2_ros_.RequestSingle();
    //// TODO: Find out what is exact time till sensors starts exposing after triggering signal was sent
    const auto expose_us = bluefox2_ros_.camera().expose_us();
    const auto expose_duration = ros::Duration(expose_us * 1e-6 / 2);
    const auto time =expose_duration;
    bluefox2_ros_.UpdateAdded2triggertime(time);

    // Pumps all published and available time stamps of the triggering signal received from ROS network into the callback function BufferTimestamp() such that time stamp
    // gets buffered
    ros::spinOnce();

    // Grab image from camera and cache it in buffer
    bluefox2_ros_.GrabandBufferImage();

    // Check whether there are cached images and corresponding time stamps in the buffer and if so stamp image and publish it in ROS network
    bluefox2_ros_.PublishImagebuffer();


    //Sleep();
  }

  // Print out which image were left in the buffer
  //bluefox2_ros_.PrintImageBuffer();
}

void SingleNode::Setup(Bluefox2DynConfig &config) {
  bluefox2_ros_.set_fps(config.fps);
  boost_ = config.boost;
  bluefox2_ros_.camera().Configure(config);
}

}  // namepace bluefox2
