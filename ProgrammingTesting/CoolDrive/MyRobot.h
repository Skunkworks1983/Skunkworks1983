#ifndef __MY_ROBOT_H
#define __MY_ROBOT_H
#include "WPILib.h"
#include "CoolDrive.h"

class RobotDemo : public SimpleRobot {
	CoolDrive *driveBase; // robot drive system
	Joystick *leftStick; // only joystick
	Joystick *rightStick;
public:
	RobotDemo();
	~RobotDemo();
	void Autonomous();
	void OperatorControl();
};
START_ROBOT_CLASS(RobotDemo);
#endif
