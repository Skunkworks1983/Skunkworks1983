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
	while (IsOperatorControl() && !IsDisabled())
	{
		count++;
		if (count/100 == (float)count/100)
		{
#if USE_PID
			cout<<"Left Speed: "<<drive->getL()<<" Left Percent: "
					<<(int)(drive->getLPercent() * 100)<<" Left Error: "
					<<drive->getLError()<<" P: "<<drive->getP()<<" Setpoint: "
					<<drive->getLSetpoint()<<endl;
#else
			cout<<"Left Speed: "<<drive->getL()<<" Right Speed: "<<drive->getR()<<endl;
#endif
		}

		//Set the compressor
		drive->updateCompressor();
		//Set the drive base to the stick speeds
		if (fabs(rStick->GetY()) > 0.02)
		{
			drive->setSpeedR(rStick->GetY());
		} else
		{
			drive->setSpeedR(0.0);
		}
		if (fabs(lStick->GetY()) > .02)
		{
			drive->setSpeedL(lStick->GetY());
		} else
		{
			drive->setSpeedL(0.0);
		}

		//check for shifting
		if (lStick->GetRawButton(1))
		{
			drive->shift(false);
		} else
		{
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

#if USE_PID
		//Check for PID modification DEBUG
		if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		{
			drive->pDown();
		}
		if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		{
			drive->pUp();
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
#endif
		Wait(.01);
	}
}

int PewPewBot::getOperatorControlMode()
{
	//
}
void PewPewBot::Disabled()
{

}

START_ROBOT_CLASS(PewPewBot)
;
