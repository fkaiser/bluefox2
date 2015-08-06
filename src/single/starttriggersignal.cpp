#include "ros/ros.h"
#include "mavros/CommandTriggerControl.h"
#include "std_msgs/Empty.h"
#include <cstdlib>
#include <string>

// This node waits for single nodes to get ready and then call CommandTriggerControl service as soon it is available.

class CamReady{

public:
	CamReady(){
		cam1_ok_=false;
		cam2_ok_=false;
		cam1_paramserver_name_="cam1";
		cam2_paramserver_name_="cam2";
		startopic_paramserver_name_="starttopic";
		getnames();
		subscribetocams();
		client_ = n_.serviceClient<mavros::CommandTriggerControl>("/mavros/cmd/trigger_control");
	}

	void cam1ready(const std_msgs::EmptyConstPtr& msg){
		cam1_ok_=true;
		ROS_INFO_STREAM("Camera " << cam1_name_<<" waits for triggering signal...");
	}

	void cam2ready(const std_msgs::EmptyConstPtr& msg){
		cam2_ok_=true;
		ROS_INFO_STREAM("Camera " << cam2_name_<<" waits for triggering signal...");
	}

	void getnames(){
		ros::param::get(cam1_paramserver_name_,cam1_name_);
		ros::param::get(cam2_paramserver_name_,cam2_name_);
		ros::param::get(startopic_paramserver_name_,topic_name_);
	}
	void subscribetocams(){
		cam1_sub_ = n_.subscribe(cam1_name_ + "/" + topic_name_,0,&CamReady::cam1ready,this);
		cam2_sub_ = n_.subscribe(cam2_name_ + "/" + topic_name_,0,&CamReady::cam2ready,this);
	}

	bool cam1_ok()
	{
		if(cam1_ok_)
		{
			return true;
		}
		return false;
	}

	bool cam2_ok()
	{
		if(cam2_ok_)
		{
			return true;
		}
		return false;
	}
int sendtriggerservicecall(){
	srv_.request.integration_time = 0;
	srv_.request.trigger_enable=true;
	 if (client_.call(srv_))
	  {
	    ROS_INFO("Successfully called service trigger_control");
	  }
	  else
	  {
	    ROS_ERROR("Failed to call service trigger_control");
	    return 1;
	  }

	  return 0;


}

private:
	bool cam1_ok_;
	bool cam2_ok_;
	std::string cam1_name_;
	std::string cam2_name_;
	std::string topic_name_;
	std::string cam1_paramserver_name_;
	std::string cam2_paramserver_name_;
	std::string startopic_paramserver_name_;
	ros::NodeHandle n_;
	ros::Subscriber cam1_sub_;
	ros::Subscriber cam2_sub_;
	ros::ServiceClient client_;
	 mavros::CommandTriggerControl srv_;
};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "StarterTriggerSignal");
  CamReady v;

  // Define time update rate to call callback function if necessary
  ros::Rate r(50); //   Number in Hz
  while(!(v.cam1_ok() && v.cam2_ok()) && ros::ok())
  {
  ros::spinOnce();
  r.sleep();

  }

  // Send  start trigger signal
  return v.sendtriggerservicecall();

}
