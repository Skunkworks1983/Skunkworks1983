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

#if USE_PID
	leftPIDSource = new C1983PIDSource(leftEncoder,MAXSPEEDHIGH,false);
	rightPIDSource = new C1983PIDSource(rightEncoder,MAXSPEEDHIGH,false);
	
	leftPIDOutput = new C1983PIDOutput(leftJag1,leftJag2,true);
	rightPIDOutput = new C1983PIDOutput(rightJag1,rightJag2,false);
	
	leftPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,leftPIDSource,leftPIDOutput);
	rightPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,rightPIDSource,rightPIDOutput);
	
	leftPID->SetOutputRange(-1.0,1.0);
	rightPID->SetOutputRange(-1.0,1.0);
	
	leftPID->SetInputRange(-1.0,1.0);
	rightPID->SetInputRange(-1.0,1.0);
	
	leftPID->Enable();
	rightPID->Enable();
#endif

	//Compressor and compressor switch. compressorSwitch reads 0 when the compressor needs to be running
	compressor = new Relay(DIGITAL_MODULE,COMPRESSOR_PORT,Relay::kBothDirections);
	compressorSwitch = new DigitalInput(DIGITAL_MODULE,COMPRESSOR_SWITCH_PORT);
	
	//Shifter
	shifter = new Relay(DIGITAL_MODULE,2);
	
	light = new Relay(DIGITAL_MODULE,1);
	
	lightSensorFront = new DigitalInput(LIGHT_SENSOR_CHANNEL_FRONT);
	lightSensorBack = new DigitalInput(LIGHT_SENSOR_CHANNEL_BACK);
	
	//We start shifted high
	shift(true);
	shiftedHigh = true;
}

//Set both jags left side to the given speed -1.0 to 1.0
void C1983Drive::setSpeedL(float speed)
{
#if USE_PID
	leftPID->SetSetpoint(-speed);
	if(speed == 0.0)
	{
		leftPID->Reset();
		leftPID->Enable();
	}
#else
	leftJag1->Set(speed);
	leftJag2->Set(speed);
#endif
}

//Set both jags right side to the negative of a given speed -1.0 to 1.0
void C1983Drive::setSpeedR(float speed)
{
#if USE_PID
	rightPID->SetSetpoint(-speed);
	if(speed == 0.0)
	{
		rightPID->Reset();
		rightPID->Enable();
	}
#else
	rightJag1->Set(-speed);
	rightJag2->Set(-speed);
#endif	
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
#if USE_PID
		leftPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		rightPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		leftPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);
		rightPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);
#endif
	//Shift Low
	}else if(!high && shiftedHigh){
		shifter->Set(Relay::kForward);
		shiftedHigh = false;
#if USE_PID
		leftPIDSource->setMaxSpeed(MAXSPEEDLOW);
		rightPIDSource->setMaxSpeed(MAXSPEEDLOW);
		leftPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);
		rightPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);
#endif
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

double C1983Drive::getL()
{
	return (float)((int)(leftEncoder->GetRate() * 100))/100;
}

double C1983Drive::getR()
{
	return (float)((int)(rightEncoder->GetRate() * 100))/100;
}
#if USE_PID
float C1983Drive::getLError()
{
	return (float)((int)(leftPID->GetError() * 100))/100;
}

float C1983Drive::getI()
{
	return leftPID->GetI();
}

float C1983Drive::getP()
{
	return leftPID->GetP();
}

float C1983Drive::getLSetpoint()
{
	return (float)((int)(leftPID->GetSetpoint() * 100))/100;
}

float C1983Drive::getRSetpoint()
{
	return (float)((int)(rightPID->GetSetpoint() * 100))/100;
}

double C1983Drive::getLPercent()
{
	return leftPIDSource->PIDGet();
}

double C1983Drive::getRPercent()
{
	return rightPIDSource->PIDGet();
}

void C1983Drive::iUp()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI() + .01,rightPID->GetD());
	leftPID->SetPID(leftPID->GetP(),leftPID->GetI() + .01,leftPID->GetD());
}
	
void C1983Drive::iDown()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI() - .01,rightPID->GetD());
	leftPID->SetPID(rightPID->GetP(),leftPID->GetI() - .01,leftPID->GetD());
}

void C1983Drive::pUp()
{
	rightPID->SetPID(rightPID->GetP() + .05,rightPID->GetI(),rightPID->GetD());
	leftPID->SetPID(leftPID->GetP() + .05,leftPID->GetI(),leftPID->GetD());
}
	
void C1983Drive::pDown()
{
	rightPID->SetPID(rightPID->GetP() - .05,rightPID->GetI(),rightPID->GetD());
	leftPID->SetPID(rightPID->GetP() - .05,leftPID->GetI(),leftPID->GetD());
}

void C1983Drive::resetLeftI()
{
	leftPID->Reset();
	leftPID->Enable();
}

void C1983Drive::resetRightI()
{
	rightPID->Reset();
	rightPID->Enable();
}

#endif
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
