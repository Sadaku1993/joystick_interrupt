//まるでcheatstart
//バツで自律に


#include <ros/ros.h>
//#include <knm_tiny_msgs/Velocity.h>
#include <geometry_msgs/Twist.h>
#include <boost/thread.hpp>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

using namespace std;


/////////////////////////////////////////////////////////
//-------------------CallBack!!!!!---------------------//
/////////////////////////////////////////////////////////
boost::mutex joy_mutex_;
//knm_tiny_msgs::Velocity cmd_;
geometry_msgs::Twist cmd_;
std_msgs::Bool cheat_flag;
bool pub_flag = false;
double gain = 0;
void joycallback(const sensor_msgs::JoyConstPtr& msg)
{
	boost::mutex::scoped_lock(joy_mutex_);

	//gain = (msg->axes[2]+1.0) * (1-msg->buttons[8]*0.5) * (1+msg->buttons[10]*0.6);
	gain = (1-msg->buttons[8]*0.5) * (1+msg->buttons[10]*0.6);

	if( msg->buttons[4] || msg->buttons[5] || msg->buttons[6] || msg->buttons[7] ){
	// if( msg->axes[9] || msg->axes[10]){
		// cmd_.linear.x  = msg->axes[9]*0.5*gain;
		// cmd_.angular.z = msg->axes[10]*0.4*gain;
	}else{
		// cmd_.linear.x=msg->axes[1]*1.0*gain;
		// cmd_.angular.z=-msg->axes[0]*1.2*gain;
		cmd_.linear.x=msg->axes[1]*0.4;
		cmd_.angular.z=msg->axes[0]*0.5;
	}

	if( msg->buttons[1] ){ pub_flag = true; }
	if( msg->buttons[0] ){ pub_flag = false; }
	cheat_flag.data = pub_flag;

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "knm_tiny_joy");

	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("/joy", 10, joycallback);
	ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cheat_velocity", 10);
	ros::Publisher flag_pub = n.advertise<std_msgs::Bool>("cheat_flag", 2);
	
	ros::Rate r(50);
	while(ros::ok()){
		if(pub_flag){
			pub.publish(cmd_);
		}
		flag_pub.publish(cheat_flag);
		ros::spinOnce();
		r.sleep();
		
	}
}

