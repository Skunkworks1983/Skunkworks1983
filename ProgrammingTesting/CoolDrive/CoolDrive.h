#ifndef __COOL_DRIVE_H
#define __COOL_DRIVE_H
#include "WPILib.h"

class CoolDrive {
	Jaguar *frontLeftMotor;
	Jaguar *rearLeftMotor;
	Jaguar *frontRightMotor;
	Jaguar *rearRightMotor;

public:
	CoolDrive(int frontLeftMotor, int rearLeftMotor, int frontRightMotor,
			int rearRightMotor);
	~CoolDrive();
	void setSpeedLeft(float speed);
	void setSpeedRight(float speed);
	void tankDrive(Joystick *leftStick, Joystick *rightStick);
	void arcadeDrive(Joystick *stick);
};

#endif
