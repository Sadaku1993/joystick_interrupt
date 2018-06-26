#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <std_msgs/Bool.h>

bool em_flag = false;
bool rp_flag = false;
bool cheat_flag = false;
bool joy_flag = false;

geometry_msgs::Twist joy_cmd_vel;
geometry_msgs::Twist cheat_vel;
geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist cheat_cmd_vel;

void em_Callback(const std_msgs::Bool::Ptr msg)
{
	em_flag = msg->data;
	rp_flag = true;
	// std::cout<<em_flag<<std::endl;
}

void cf_Callback(const std_msgs::Bool::Ptr msg)
{
	cheat_flag = msg->data;
	// std::cout<<cheat_flag<<std::endl;
}

void JoyCallback(const geometry_msgs::Twist::Ptr msg)
{
	joy_cmd_vel.linear.x = msg->linear.x;
	joy_cmd_vel.angular.z = msg->angular.z;
	joy_flag = true;
}

void CheatCallback(const geometry_msgs::Twist::Ptr msg)
{
	cheat_vel.linear.x = msg->linear.x;
	cheat_vel.angular.z = msg->angular.z;
	// cheat_flag = true;
}

int main(int argc, char *argv[])
{
	ros::init(argc, argv, "emergency_interrupt");

	ros::NodeHandle n;
	ros::Subscriber em_sub = n.subscribe("/emergency_stop", 2, em_Callback);
	ros::Subscriber cf_sub = n.subscribe("/cheat_flag", 2, cf_Callback);
	ros::Subscriber joy_sub = n.subscribe("/interrupt_cmd_vel", 2, JoyCallback);
	ros::Subscriber cheat_sub = n.subscribe("/cheat_velocity",2, CheatCallback);
	ros::Publisher vel_pub = n.advertise<geometry_msgs::Twist>("/t_frog/cmd_vel",2);
	ros::Publisher cheat_vel_pub = n.advertise<geometry_msgs::Twist>("/t_frog/cheat_velocity",2);

	ros::Rate r(50);
	while(ros::ok()){
		if(em_flag || !rp_flag){
			cmd_vel.linear.x = 0.0;
			cmd_vel.angular.z = 0.0;
			cheat_cmd_vel.linear.x = 0.0;
			cheat_cmd_vel.angular.z = 0.0;
			vel_pub.publish(cmd_vel);
			cheat_vel_pub.publish(cheat_cmd_vel);
		}
		else if(joy_flag){
		// if(joy_flag){
		// if(joy_flag){
			cmd_vel.linear.x = joy_cmd_vel.linear.x;
			cmd_vel.angular.z = joy_cmd_vel.angular.z;
			vel_pub.publish(cmd_vel);
		}
		else if(cheat_flag){
			cheat_cmd_vel.linear.x = cheat_vel.linear.x;
			cheat_cmd_vel.angular.z = cheat_vel.angular.z;
			cheat_vel_pub.publish(cheat_cmd_vel);
		}
		// else{
		// 	cmd_vel.linear.x = 0.0;
		// 	cmd_vel.angular.z = 0.0;
		// 	cheat_cmd_vel.linear.x = 0.0;
		// 	cheat_cmd_vel.angular.z = 0.0;
		// 	vel_pub.publish(cmd_vel);
		// 	cheat_vel_pub.publish(cheat_cmd_vel);
		// }


		joy_flag = false;
		// rp_flag = false;
		ros::spinOnce();
		r.sleep();
	}
}
