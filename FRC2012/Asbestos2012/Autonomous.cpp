#include "PewPewBot.h"
#define GYROTEST 1
void PewPewBot::Autonomous()
{
	drive->shift(true);
#if GYROTEST
	int count = 0;
	int phase = 0;
	drive->resetEncoders();
#endif
	while (IsAutonomous() && !IsDisabled())
	{
		count++;
		if(count/10 == (float)count/10) cout<<"Phase: "<<phase<<" LEnc: "<<drive->getLPosition()<<" REnc: "<<drive->getRPosition()<<" Gyro: "<<drive->getGyro()<<endl;
		drive->updateCompressor();
#if !KINECT
#if GYROTEST
		switch(phase)
		{
		case 0:
			if((drive->getLPosition() + drive->getRPosition())/2 < 50)
			{
				drive->setSpeedL(0.2);
				drive->setSpeedR(0.2);
			}else{
				phase++;
				drive->resetGyro();
			}
			break;
		case 1:
			if(drive->getGyro() < 165)
			{
				drive->setSpeedR(-0.2);
				drive->setSpeedL(0.2);
			}else{
				phase++;
				drive->resetEncoders();
			}
			break;
		case 2:
			if((drive->getLPosition() + drive->getRPosition())/2 < 50)
			{
				drive->setSpeedL(0.2);
				drive->setSpeedR(0.2);
			}else{
				phase++;
				drive->resetGyro();
			}
			break;
		case 3:
			if(drive->getGyro() < 165)
			{
				drive->setSpeedR(-0.2);
				drive->setSpeedL(0.2);
			}else{
				phase++;
			}
			break;
		case 4:
			drive->setSpeedR(0.0);
			drive->setSpeedL(0.0);
		}
#else
		lineDepthAlign();
#endif
#else	
		kinectCode();
#endif
		Wait(0.02);
	}
}

int PewPewBot::getAutonomousMode()
{
	//Return an AutomaticMode enum
	return 0;
}

#if KINECT
void PewPewBot::kinectCode()
{
	if (fabs(kinect->getRight())> 0.05)
	{
		drive->setSpeedR(kinect->getRight());
	} else
	{
		drive->setSpeedR(0.0);
	}
	if (fabs(kinect->getLeft())> 0.05)
	{
		drive->setSpeedL(kinect->getLeft());
	} else
	{
		drive->setSpeedL(0.0);
	}

	//Shift Stuff
	if (kinect->getShiftButton())
	{
		drive->shift(true);
	} else
	{
		drive->shift(false);
	}

	//Check whether we're close enough to the setpoint. If so, reset I.
	if (fabs(drive->getLSetpoint() - drive->getL()) <= .01)
		drive->resetLeftI();

	if (fabs(drive->getRSetpoint() - drive->getR()) <= .01)
		drive->resetRightI();
}
#endif
