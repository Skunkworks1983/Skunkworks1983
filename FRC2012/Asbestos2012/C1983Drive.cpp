#include "C1983Drive.h"
#include <iostream>

C1983Drive::C1983Drive()
{
	leftJag1 = new Jaguar(JAG_PORT_LEFT_1);
	leftJag2 = new Jaguar(JAG_PORT_LEFT_2);
	rightJag1 = new Jaguar(JAG_PORT_RIGHT_1);
	rightJag2 = new Jaguar(JAG_PORT_RIGHT_2);
	
	leftEncoder = new Encoder(LEFT_ENCODER_PORT_A,LEFT_ENCODER_PORT_B,true);
	rightEncoder = new Encoder(RIGHT_ENCODER_PORT_A,RIGHT_ENCODER_PORT_B);
	
	//initialize encoders
	leftEncoder->Start();
	rightEncoder->Start();
	
	//Tell encoders to use rate for PIDGet()
	leftEncoder->SetPIDSourceParameter(Encoder::kRate);
	rightEncoder->SetPIDSourceParameter(Encoder::kRate);
	
	leftEncoder->SetDistancePerPulse(0.01925);
	rightEncoder->SetDistancePerPulse(0.01925);

	leftPIDSource = new C1983PIDSource(leftEncoder,MAXSPEEDHIGH,false);
	rightPIDSource = new C1983PIDSource(rightEncoder,MAXSPEEDHIGH,false);
	
	//Left is reversed, right isn't
	leftPIDOutput = new C1983PIDOutput(leftJag1,leftJag2,true);
	rightPIDOutput = new C1983PIDOutput(rightJag1,rightJag2,false);
	
	leftPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,leftPIDSource,leftPIDOutput);
	rightPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,rightPIDSource,rightPIDOutput);
	
	leftPID->SetOutputRange(-1.0,1.0);
	rightPID->SetOutputRange(-1.0,1.0);
	
	leftPID->SetInputRange(-1.0,1.0);
	rightPID->SetInputRange(-1.0,1.0);

	//Compressor and compressor switch. compressorSwitch reads 0 when the compressor needs to be running
	compressor = new Relay(DIGITAL_MODULE,COMPRESSOR_PORT,Relay::kBothDirections);
	compressorSwitch = new DigitalInput(DIGITAL_MODULE,COMPRESSOR_SWITCH_PORT);
	
	//Shifter
	shifter = new Relay(DIGITAL_MODULE,2);
	
	light = new Relay(DIGITAL_MODULE,1);
	
	lightSensorFront = new DigitalInput(LIGHT_SENSOR_CHANNEL_FRONT);
	lightSensorBack = new DigitalInput(LIGHT_SENSOR_CHANNEL_BACK);
	
	gyro = new Gyro(GYRO_CHANNEL);
	//We start shifted high
	shift(true);
	shiftedHigh = true;
}

//Set both jags left side to the given speed -1.0 to 1.0
void C1983Drive::setSpeedL(float speed)
{
	leftPID->SetSetpoint(speed);
	if(fabs(leftPID->GetSetpoint())/leftPID->GetSetpoint() != fabs(speed)/speed || speed == 0.0)
	{
		resetLeftI();
	}
}

//Set both jags right side to the negative of a given speed -1.0 to 1.0
void C1983Drive::setSpeedR(float speed)
{
	if(fabs(rightPID->GetSetpoint())/rightPID->GetSetpoint() != fabs(speed)/speed || speed == 0.0)
	{
		resetRightI();
	}
	rightPID->SetSetpoint(speed);
}

void C1983Drive::updateCompressor()
{
	if(!compressorSwitch->Get()){
		compressor->Set(Relay::kReverse);
	}else{
		compressor->Set(Relay::kOff);
	}
}


/**If high is true, will shift high (if not already shifted high) and vice versa.
 * kForward shifts low
 **/

void C1983Drive::shift(bool high)
{
	//Shift high
	if(high && !shiftedHigh)
	{
		shifter->Set(Relay::kReverse);
		shiftedHigh = true;

		leftPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		rightPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		leftPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);
		rightPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);

	//Shift Low
	}else if(!high && shiftedHigh){
		shifter->Set(Relay::kForward);
		shiftedHigh = false;

		leftPIDSource->setMaxSpeed(MAXSPEEDLOW);
		rightPIDSource->setMaxSpeed(MAXSPEEDLOW);
		leftPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);
		rightPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);

	}else{
		return;
	}
}

void C1983Drive::setLight(bool on)
{
	if(on)
	{
		light->Set(Relay::kForward);
	}else{
		light->Set(Relay::kOff);
	}
}

bool C1983Drive::getLightSensorBack()
{
	return (bool)(lightSensorBack->Get());
}

bool C1983Drive::getLightSensorFront()
{
	return (bool)(lightSensorFront->Get());
}

float C1983Drive::getGyro()
{
	return gyro->GetAngle();
}

void C1983Drive::resetGyro()
{
	gyro->Reset();
}


