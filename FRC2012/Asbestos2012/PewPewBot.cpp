#include "PewPewBot.h"
#include <iostream>
#include <math.h>

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

void PewPewBot::Autonomous()
{
	while (IsAutonomous() && IsEnabled())
	{
#if KINECT
		printf("Left: %f\tRight: %f\n", kinect->getLeft(), kinect->getRight());
		drive->setSpeedR(kinect->getRight());
		drive->setSpeedL(kinect->getLeft());
#endif
	}

}

void PewPewBot::OperatorControl()
{
	int count = 0;//DEBUG (C1983)
	while (IsOperatorControl() && !IsDisabled())
	{
		count++;
		if (count/150 == (float)count/150)
		{
#if USE_PID
			cout<<"Left Speed: "<<drive->getL()<<" Left Percent: "<<(int)(drive->getLPercent() * 100)<<" Left Error: "<<drive->getLError()<<" I: "<<drive->getI()<<" Setpoint: "<<drive->getLSetpoint()<<endl;
#endif
		}

		//Set the compressor
		drive->updateCompressor();
		//Set the drive base to the stick speeds
		if(fabs(rStick->GetY()) > 0.05)
		{
			drive->setSpeedR(rStick->GetY());
		}else{
			drive->setSpeedR(0.0);
		}
		if(fabs(lStick->GetY()) > .05)
		{
			drive->setSpeedL(lStick->GetY());
		}else{
			drive->setSpeedL(0.0);
		}
			
#if USE_PID
		if (count/10 == (float)count/10)
		{
			if (rStick->GetRawButton(2))
				drive->setSpeedL(drive->getLSetpoint() - 0.2);
			if (rStick->GetRawButton(3))
				drive->setSpeedL(drive->getLSetpoint() + 0.2);
			if (rStick->GetRawButton(4))
				drive->setSpeedL(0.0);
		}

		//Check for shifting
		if (lStick->GetRawButton(1))
		{
			drive->shift(false);
		} else
		{
			drive->shift(true);
		}

		//Check for PID modification DEBUG
		if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		{
			drive->iDown();
		}
		if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		{
			drive->iUp();
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

void PewPewBot::Disabled()
{

}

START_ROBOT_CLASS(PewPewBot)
;
