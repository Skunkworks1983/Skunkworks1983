#include "PewPewBot.h"
#include <iostream>

PewPewBot::PewPewBot()
{
	drive = new C1983Drive();
#if KINECT
	kinect = new C1983Kinect();
#endif
	lStick = new Joystick(1);
	rStick = new Joystick(2);
}

PewPewBot::~PewPewBot()
{
}

void PewPewBot::OperatorControl()
{
	int count = 0;//DEBUG (C1983)
	drive->enablePID();
	while (IsOperatorControl() && !IsDisabled())
	{
		count++;
		if (count/25 == (float)count/25)
		{
			cout<<"SetpointL: "<<drive->getLSetpoint()<<" ErrorL: "<<drive->getLError()<<" LPercent: "<<drive->getLPercent()<<" D: "<<drive->getD()<<endl;
		}

		//Set the compressor
		drive->updateCompressor();
		//Set the drive base to the stick speeds (Joysticks are backwards yo!)
		if (fabs(rStick->GetY()) > 0.02)
		{
			drive->setSpeedR(-rStick->GetY());
		}else{
			drive->setSpeedR(0.0);
		}
		if (fabs(lStick->GetY()) > .02)
		{
			drive->setSpeedL(-lStick->GetY());
		} else{
			drive->setSpeedL(0.0);
		}

		//check for shifting
		if (lStick->GetRawButton(1))
		{
			drive->shift(false);
		}else{
			drive->shift(true);
		}

		//check for light
		if (rStick->GetRawButton(1))
		{
			drive->setLight(true);
		} else
		{
			drive->setLight(false);
		}

		if(lStick->GetRawButton(10))
		{
			drive->resetGyro();
		}
		
		//Check for PID modification DEBUG
		if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		{
			drive->dDown();
		}
		if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		{
			drive->dUp();
		}
		if (lStick->GetRawButton(5))
		{
			drive->resetLeftI();
			drive->resetRightI();
		}

		if (fabs(drive->getLSetpoint()- drive->getL()) <= .01)
			drive->resetLeftI();

		if (fabs(drive->getRSetpoint()- drive->getR()) <= .01)
			drive->resetRightI();

		Wait(.01);
	}
}

int PewPewBot::getOperatorControlMode()
{
	//
	return 0;
}

void PewPewBot::Disabled()
{
	drive->resetEncoders();
	drive->cleanPID();
	while(IsDisabled())
	{
		Wait(0.1);
	}
}

START_ROBOT_CLASS(PewPewBot);
