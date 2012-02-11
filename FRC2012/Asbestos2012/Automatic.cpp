#include "PewPewBot.h"

/*
 * This contains code that is shared between the autonomous and operator modes
 */
void PewPewBot::automaticMode(int autoMode)
{
	switch (autoMode)
	{
	case kLineDepthAlign:
		lineDepthAlign();
		break;
	}
}
void PewPewBot::lineDepthAlign()
{
	if (drive->getLightSensorBack() && drive->getLightSensorFront()) //We are on the key, backwards
	{
		drive->setSpeedL(LINE_STOP_SPEED);
		drive->setSpeedR(LINE_STOP_SPEED);
	} else if (!drive->getLightSensorFront() && !drive->getLightSensorBack())
	{
		drive->setSpeedL(-LINE_STOP_SPEED);
		drive->setSpeedR(-LINE_STOP_SPEED);
	} else
	{
		drive->setSpeedL(0);
		drive->setSpeedR(0);
	}
}
