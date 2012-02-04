#include "PewPewBot.h"
void PewPewBot::Autonomous()
{
	drive->shift(true);
	int count = 0;
	while (IsAutonomous() && IsEnabled())
	{
		drive->updateCompressor();
#if !KINECT
		lineDepthAlign();
#else	
		kinectCode();
#endif
		Wait(0.1);
	}
}

int PewPewBot::getAutonomousMode()
{
	//Return an AutomaticMode enum
}

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
