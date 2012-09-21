#include "C1983Drive.h"
#include <iostream>

C1983Drive::C1983Drive()
{
#if !(GOGO)
	leftVic1 = new Victor(VIC_PORT_LEFT_1);
#endif
	leftVic2 = new Victor(VIC_PORT_LEFT_2);
	rightVic1 = new Victor(VIC_PORT_RIGHT_1);
	rightVic2 = new Victor(VIC_PORT_RIGHT_2);

	leftEncoder
			= new Encoder(ENCODER_SLOT,LEFT_ENCODER_PORT_A,ENCODER_SLOT,LEFT_ENCODER_PORT_B,true,Encoder::k4X);
	rightEncoder
			= new Encoder(ENCODER_SLOT,RIGHT_ENCODER_PORT_A,ENCODER_SLOT,RIGHT_ENCODER_PORT_B,false,Encoder::k4X);

	leftEncoder->Reset();
	rightEncoder->Reset();

	//initialize encoders
	leftEncoder->Start();
	rightEncoder->Start();

	//Tell encoders to use rate for PIDGet()
	leftEncoder->SetPIDSourceParameter(Encoder::kRate);
	rightEncoder->SetPIDSourceParameter(Encoder::kRate);

	leftEncoder->SetDistancePerPulse(FEET_PER_TICK_HIGH);
	rightEncoder->SetDistancePerPulse(FEET_PER_TICK_HIGH);

#if DRIVE_PID
	leftPIDSource = new C1983PIDSource(leftEncoder,MAXSPEEDHIGH,false);
	rightPIDSource = new C1983PIDSource(rightEncoder,MAXSPEEDHIGH,false);

	//right is reversed, left isn't
#if GOGO
	leftPIDOutput = new C1983PIDOutput(leftVic2,leftVic2,false);
#else
	leftPIDOutput = new C1983PIDOutput(leftVic1,leftVic2,false);
#endif
	rightPIDOutput = new C1983PIDOutput(rightVic1,rightVic2,true);

	leftPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,leftPIDSource,leftPIDOutput);
	rightPID = new PIDController(DRIVE_P,DRIVE_I,DRIVE_D,rightPIDSource,rightPIDOutput);

	leftPID->SetOutputRange(-1.0, 1.0);
	rightPID->SetOutputRange(-1.0, 1.0);

	leftPID->SetInputRange(-1.0, 1.0);
	rightPID->SetInputRange(-1.0, 1.0);
#endif
	gyro = new Gyro(GYRO_CHANNEL);

	//Turn PID Begin
	turnPIDSource = new C1983TurnPIDSource(gyro);
#if !(GOGO)
	turnPIDOutput = new C1983TurnPIDOutput(leftVic1,leftVic2,rightVic1,rightVic2);
#else
	turnPIDOutput = new C1983TurnPIDOutput(leftVic2,leftVic2,rightVic1,rightVic2);
#endif
	turnPID = new PIDController(TURN_P,TURN_I,TURN_D,turnPIDSource,turnPIDOutput);
	turnPID->SetOutputRange(-1.0, 1.0);
	turnPID->SetInputRange(-360, 360);
	//Turn PID End

	//Compressor and compressor switch. compressorSwitch reads 0 when the compressor needs to be running
	compressor = new Relay(DIGITAL_MODULE,COMPRESSOR_PORT,Relay::kBothDirections);
	compressorSwitch = new DigitalInput(DIGITAL_MODULE,COMPRESSOR_SWITCH_PORT);

	//Shifter
	shiftHigh = new Solenoid(SOLENOID_MODULE,SHIFTER_CHANNEL);
	shiftLow = new Solenoid(SOLENOID_MODULE,SHIFTER_LOW_CHANNEL);

	pokerUp = new Solenoid(SOLENOID_MODULE,POKER_CHANNEL_B);
	pokerDown = new Solenoid(SOLENOID_MODULE,POKER_CHANNEL);
	
	tipper = new C1983Tipper();

	light = new Relay(DIGITAL_MODULE,LIGHT_CHANNEL);

	lightSensorFront = new DigitalInput(LIGHT_SENSOR_CHANNEL_FRONT);
	lightSensorBack = new DigitalInput(LIGHT_SENSOR_CHANNEL_BACK);
	lightSensorBridge = new DigitalInput(LIGHT_SENSOR_CHANNEL_BRIDGE);

	//We start shifted high
	shiftedHigh = false;
	lightState = false;
	setLight(false);
	shift(true);
	poke(true);
}

//Set both vicss left side to the given speed -1.0 to 1.0
void C1983Drive::setSpeedL(float speed)
{
#if DRIVE_PID
	if (fabs(speed) < DEADBAND && fabs(leftPID->GetSetpoint()) > DEADBAND)
	{
		resetLeftI();
	}
	leftPID->SetSetpoint(speed);
#else
	leftVic1->Set(speed);
	leftVic2->Set(speed);
#endif
}

//Set both vics right side to the negative of a given speed -1.0 to 1.0
void C1983Drive::setSpeedR(float speed)
{
#if DRIVE_PID
	if (fabs(speed) < DEADBAND && fabs(rightPID->GetSetpoint()) > DEADBAND)
	{
		resetRightI();
	}
	rightPID->SetSetpoint(speed);
#else
	rightVic1->Set(-speed);
	rightVic2->Set(-speed);
#endif
}

void C1983Drive::updateCompressor()
{
	if (!compressorSwitch->Get())
	{
		compressor->Set(Relay::kReverse);
	} else
	{
		compressor->Set(Relay::kOff);
	}
}

/**If high is true, will shift high (if not already shifted high) and vice versa.
 * kForward shifts low
 **/

void C1983Drive::shift(bool high)
{
	//Shift high
	if (high && !shiftedHigh)
	{
		shiftHigh->Set(true);
		shiftLow->Set(false);
		shiftedHigh = true;
#if DRIVE_PID
		leftPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		rightPIDSource->setMaxSpeed(MAXSPEEDHIGH);
		leftPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);
		rightPID->SetPID(DRIVE_P,DRIVE_I,DRIVE_D);
#endif		
		//Fix the distance per pulse to account for the new reduction
		leftEncoder->SetDistancePerPulse(FEET_PER_TICK_HIGH);
		rightEncoder->SetDistancePerPulse(FEET_PER_TICK_HIGH);

		//Shift Low
	} else if (!high && shiftedHigh)
	{
		shiftHigh->Set(false);
		shiftLow->Set(true);

		shiftedHigh = false;
#if DRIVE_PID
		leftPIDSource->setMaxSpeed(MAXSPEEDLOW);
		rightPIDSource->setMaxSpeed(MAXSPEEDLOW);
		leftPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);
		rightPID->SetPID(DRIVE_P_LOW,DRIVE_I_LOW,DRIVE_D_LOW);
#endif
		//Fix the distance per pulse to account for the new reduction
		leftEncoder->SetDistancePerPulse(FEET_PER_TICK_LOW);
		rightEncoder->SetDistancePerPulse(FEET_PER_TICK_LOW);

	} else
	{
		return;
	}
}

void C1983Drive::poke(bool up)
{
	if(up && !pokedUp)
	{
		pokerUp->Set(true);
		pokerDown->Set(false);
		pokedUp = true;
	}else if(!up && pokedUp){
		pokerUp->Set(false);
		pokerDown->Set(true);
		pokedUp = false;
	}else{
		return;
	}
}

bool C1983Drive::isShiftedHigh()
{
	return shiftedHigh;
}

void C1983Drive::tip(bool down)
{
	tipper->tip(down);
}

void C1983Drive::setLight(bool on)
{
	/*
	lightState = on;
	if (on) //TODO Enable the light
	{
		light->Set(Relay::kForward);
	} else
	{
		light->Set(Relay::kOff);
	}*/
}

bool C1983Drive::getLight(){
	return lightState;
}

bool C1983Drive::getLightSensorBack()
{
	return (bool)(lightSensorBack->Get());
}

bool C1983Drive::getLightSensorFront()
{
	return (bool)(lightSensorFront->Get());
}

bool C1983Drive::getLightSensorBridge()
{
	return (bool)(lightSensorBridge->Get());
}

float C1983Drive::getGyro()
{
	return gyro->GetAngle();
}

void C1983Drive::resetGyro()
{
	gyro->Reset();
}

