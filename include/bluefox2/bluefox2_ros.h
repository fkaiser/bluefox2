#ifndef BLUEFOX2_ROS_H_
#define BLUEFOX2_ROS_H_

#include "bluefox2/bluefox2.h"
#include "camera_base/camera_ros_base.h"

namespace bluefox2 {

class Bluefox2Ros : public camera_base::CameraRosBase {
 public:
  Bluefox2Ros(const ros::NodeHandle& nh,
              const std::string& prefix = std::string());

  void RequestSingle() const { bluefox2_.RequestSingle(); }
  void AwaitfirstTrigger()  {bluefox2_.AwaitfirstTrigger();}
  void Preparefortrigger(int request_time,int expose_us)const {bluefox2_.Timesettings(request_time,expose_us);}
  void SetforCapture(int request_time,int expose_us)const {bluefox2_.Timesettings(request_time,expose_us);}
  bool GetSingleImage();




  Bluefox2& camera() { return bluefox2_; }
  bool Grab(const sensor_msgs::ImagePtr& image_msg,
            const sensor_msgs::CameraInfoPtr& cinfo_msg) override;

 private:
  Bluefox2 bluefox2_;
};

}  // namespace bluefox2

#endif  // BLUEFOX2_ROS_H_
