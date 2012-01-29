#include "C1983Drive.h"
#include <iostream>

C1983Drive::C1983Drive()
{
	leftJag1 = new Jaguar(JAG_PORT_LEFT_1);
	leftJag2 = new Jaguar(JAG_PORT_LEFT_2);
	rightJag1 = new Jaguar(JAG_PORT_RIGHT_1);
	rightJag2 = new Jaguar(JAG_PORT_RIGHT_2);
	
	//fakeEncoder1 = new Encoder(1,12,1,13,true,Encoder::k4X);
	leftEncoder = new Encoder(LEFT_ENCODER_PORT_A,LEFT_ENCODER_PORT_B);
	//fakeEncoder2 = new Encoder(1,9,1,10,true,Encoder::k4X);
	rightEncoder = new Encoder(RIGHT_ENCODER_PORT_A,RIGHT_ENCODER_PORT_B);
	
	//initialize encoders
	leftEncoder->Start();
	rightEncoder->Start();
	
#if USE_PID
	leftPIDOutput = new C1983PIDOutput(leftJag1,leftJag2);
	rightPIDOutput = new C1983PIDOutput(rightJag1,rightJag2);
	
	leftPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,leftEncoder,leftPIDOutput);
	rightPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,rightEncoder,rightPIDOutput);
#endif

	//Compressor and compressor switch. compressorSwitch reads 0 when the compressor needs to be running
	compressor = new Relay(DIGITAL_MODULE,COMPRESSOR_PORT,Relay::kBothDirections);
	compressorSwitch = new DigitalInput(DIGITAL_MODULE,COMPRESSOR_SWITCH_PORT);
	
	//Shifter
	shifter = new Relay(DIGITAL_MODULE,2);
	
	lightSensor = new DigitalInput(LIGHT_SENSOR_CHANNEL);
	
	//We start shifted high
	shift(true);
	shiftedHigh = true;
}

//Set both jags left side to the given speed -1.0 to 1.0
void C1983Drive::setSpeedL(float speed)
{
#if USE_PID
	leftPID->SetSetpoint(speed);
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
	//Shift Low
	}else if(!high && shiftedHigh){
		shifter->Set(Relay::kForward);
		shiftedHigh = false;
	}else{
		return;
	}
}

int C1983Drive::getL()
{
	return leftEncoder->GetRaw();
}

int C1983Drive::getR()
{
	return rightEncoder->GetRaw();
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
