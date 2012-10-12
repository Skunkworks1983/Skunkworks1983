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
	driveBase = new CoolDrive(FRONT_LEFT_MOTOR, REAR_LEFT_MOTOR, FRONT_RIGHT_MOTOR, REAR_RIGHT_MOTOR);
	// these must be initialized in the same order
	leftStick = new Joystick(LEFT_JOYSTICK);
	rightStick = new Joystick(RIGHT_JOYSTICK);
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
		driveBase->arcadeDrive(leftStick);
	}
}
