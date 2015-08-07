#include "ros/ros.h"
#include "mavros/CommandTriggerControl.h"
#include "std_msgs/Empty.h"
#include <cstdlib>
#include <string>
#include <std_srvs/Trigger.h>
// This node waits for single nodes to get ready and then call CommandTriggerControl service as soon it is available.

class CamReady{

public:
	CamReady(){
		cam1_ok_=false;
		cam2_ok_=false;
		cam1_paramserver_name_="~cam1";
		cam2_paramserver_name_="~cam2";
		starttopic_paramserver_name_="~starttopic";
		getnames();
		subscribetocams();
		client_ = n_.serviceClient<mavros::CommandTriggerControl>("/mavros/cmd/trigger_control");
		advertisecamservice();
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
		ros::param::get(starttopic_paramserver_name_,topic_name_);
	}
	void subscribetocams(){
		cam1_sub_ = n_.subscribe(cam1_name_ + "/" + topic_name_,0,&CamReady::cam1ready,this);
		cam2_sub_ = n_.subscribe(cam2_name_ + "/" + topic_name_,0,&CamReady::cam2ready,this);
	}

	bool servcam1(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &resp){
		cam1_ok_=true;
		resp.success=true;
		ROS_INFO_STREAM("Camera " << cam1_name_<<" sent start ready-for-trigger signal");
		return true;

	}

	bool servcam2(std_srvs::Trigger::Request &req,std_srvs::Trigger::Response &resp){
		cam2_ok_=true;
		resp.success=true;
		return true;

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
void advertisecamservice()
{
	server_cam1_=n_.advertiseService(cam1_name_ + "/" + topic_name_, &CamReady::servcam1,this);
	server_cam2_=n_.advertiseService(cam2_name_ + "/" + topic_name_, &CamReady::servcam2,this);
}


private:
	bool cam1_ok_;
	bool cam2_ok_;
	std::string cam1_name_;
	std::string cam2_name_;
	std::string topic_name_;
	std::string cam1_paramserver_name_;
	std::string cam2_paramserver_name_;
	std::string starttopic_paramserver_name_;
	ros::NodeHandle n_;
	ros::Subscriber cam1_sub_;
	ros::Subscriber cam2_sub_;
	ros::ServiceClient client_;
	mavros::CommandTriggerControl srv_;
	ros::ServiceServer server_cam1_;
	ros::ServiceServer server_cam2_;


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
  ros::Rate r2(10); //   Number in Hz
  while(v.sendtriggerservicecall() && ros::ok())
  {
	  ROS_INFO_STREAM("Retrying reaching pixhawk");
	  r2.sleep();
  }

}
