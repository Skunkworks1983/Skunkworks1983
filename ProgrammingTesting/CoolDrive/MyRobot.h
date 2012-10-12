#ifndef __MY_ROBOT_H
#define __MY_ROBOT_H
#include "WPILib.h"
#include "CoolDrive.h"

#define FRONT_LEFT_MOTOR 1
#define REAR_LEFT_MOTOR 2
#define FRONT_RIGHT_MOTOR 3
#define REAR_RIGHT_MOTOR 4

#define LEFT_JOYSTICK 1
#define RIGHT_JOYSTICK 2


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
