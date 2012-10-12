#include "CoolDrive.h"

CoolDrive::CoolDrive(int frontLeftMotor, int rearLeftMotor,
		int frontRightMotor, int rearRightMotor) {
	this->frontLeftMotor = new Jaguar(frontLeftMotor);
	this->rearLeftMotor = new Jaguar(rearLeftMotor);
	this->frontRightMotor = new Jaguar(frontRightMotor);
	this->rearRightMotor = new Jaguar(rearRightMotor);
}

CoolDrive::~CoolDrive() {
	delete frontLeftMotor;
	delete rearLeftMotor;
	delete frontRightMotor;
	delete rearRightMotor;
}

void CoolDrive::setSpeedLeft(float speed) {
	frontLeftMotor->Set(speed);
	rearLeftMotor->Set(speed);

}

void CoolDrive::setSpeedRight(float speed) {
	frontRightMotor->Set(-speed);
	rearRightMotor->Set(-speed);
}

void CoolDrive::tankDrive(Joystick *leftStick, Joystick *rightStick) {
	setSpeedLeft(leftStick->GetAxis(leftStick->kYAxis));
	setSpeedRight(rightStick->GetAxis(rightStick->kYAxis));
}