#include "C1983Drive.h"
C1983Drive::C1983Drive()
{
	leftJag1 = new Jaguar(JAG_PORT_LEFT_1);
	leftJag2 = new Jaguar(JAG_PORT_LEFT_2);
	rightJag1 = new Jaguar(JAG_PORT_RIGHT_1);
	rightJag2 = new Jaguar(JAG_PORT_RIGHT_2);
	
	leftEncoder = new Encoder(LEFT_ENCODER_PORT_A,LEFT_ENCODER_PORT_B);
	rightEncoder = new Encoder(RIGHT_ENCODER_PORT_A,RIGHT_ENCODER_PORT_B);
	
	leftPIDOutput = new C1983PIDOutput(leftJag1,leftJag2);
	rightPIDOutput = new C1983PIDOutput(rightJag1,rightJag2);
	
	leftPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,leftEncoder,leftPIDOutput);
	rightPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,rightEncoder,rightPIDOutput);
}

//Set both jags left side to the given speed -1.0 to 1.0
void C1983Drive::setSpeedL(float speed)
{
	leftPID->SetSetpoitn(speed);
}

//Set both jags right side to the negative of a given speed -1.0 to 1.0
void C1983Drive::setSpeedR(float speed)
{
	leftPID->SetSetpoint(-speed);
}
/*
//TODO: Replace get function with something that actually gets speed
C1983Drive::getSpeedR()
{
	return (rightJag1->Get() + rightJag2->Get());
}

//TODO: Replace get function with something that actually gets speed
C1983Drive::getSpeedL()
{
	return (leftJag1->Get() + leftJag2->Get())/2;
};
*/
