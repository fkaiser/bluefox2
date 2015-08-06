#include "ros/ros.h"
#include "mavros/CommandTriggerControl.h"
#include <cstdlib>

// This node waits for single nodes to get ready and then call CommandTriggerControl service as soon it is available.

int main(int argc, char **argv)
{
  ros::init(argc, argv, "StarterTriggerSignal");
  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<mavros::CommandTriggerControl>("/mavros/cmd/trigger_control");
  mavros::CommandTriggerControl srv;
  srv.request.integration_time = 0;
  srv.request.trigger_enable =true;
  if (client.call(srv))
  {
    ROS_INFO("Sum: %ld",srv.response.success);
  }
  else
  {
    ROS_ERROR("Failed to call service trigger_control");
    return 1;
  }

  return 0;
}
