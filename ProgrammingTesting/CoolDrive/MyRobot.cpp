#include "MyRobot.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */

RobotDemo::RobotDemo()
// as they are declared above.
{
	driveBase = new CoolDrive(1, 2, 3, 4); // these must be initialized in the same order
	leftStick = new Joystick(1);
	rightStick = new Joystick(2);
}

RobotDemo::~RobotDemo() {
	delete driveBase;
	delete leftStick;
	delete rightStick;
}

/**
 * Drive left & right motors for 2 seconds then stop
 */
void RobotDemo::Autonomous() {
}

/**
 * Runs the motors with arcade steering. 
 */
void RobotDemo::OperatorControl() {
	while (IsOperatorControl()) {
		driveBase->tankDrive(leftStick, rightStick);
	}
}