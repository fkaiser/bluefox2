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

// Inform start trigger node that camera is ready and waits for trigger signal
  ros::Rate r(10); //   Number in Hz

  // Prepare camera setting such that it will can catch first trigger event

  bluefox2_ros_.Preparefortrigger(0,12);


 // Call ready-for-trigger service
while(!bluefox2_ros_.SendReadyforTrigger() && ros::ok())
{
	ROS_INFO_STREAM("Retrying");
	r.sleep();
}

// Wait for first trigger signal
bluefox2_ros_.AwaitfirstTrigger();

bluefox2_ros_.CallCallbackOnce();

// Set parameters for capturing images
bluefox2_ros_.SetforCapture(0,5000);


ros::Rate r_loop(35);
 while (is_acquire() && ros::ok()) {

	 // Try to get an image
	 if(bluefox2_ros_.GetSingleImage()){

	// Take corresponding time stamp and publish image TODO: Add exposure time to time stamp
		bluefox2_ros_.CallCallbackOnce();
	 }

  }

}

void SingleNode::Setup(Bluefox2DynConfig &config) {
  bluefox2_ros_.set_fps(config.fps);
  boost_ = config.boost;
  bluefox2_ros_.camera().Configure(config);
}

}  // namepace bluefox2
