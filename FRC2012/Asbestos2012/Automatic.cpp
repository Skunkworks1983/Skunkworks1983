#include "PewPewBot.h"

bool PewPewBot::lineDepthAlign()
{
	if (drive->getLightSensorBack() && drive->getLightSensorFront()) //We are on the key, forwards
	{
		drive->setSpeedL(LINE_STOP_SPEED);
		drive->setSpeedR(LINE_STOP_SPEED);
		stableCount = 0;
	} else if (!drive->getLightSensorFront() && !drive->getLightSensorBack()) //We are off the key, backwards
	{
		drive->setSpeedL(-LINE_STOP_SPEED);
		drive->setSpeedR(-LINE_STOP_SPEED);
		stableCount = 0;
	} else
	{
		drive->setSpeedL(0);
		drive->setSpeedR(0);
		if (stableCount++ >= 10)
			return true;
	}
	return false;
}

bool PewPewBot::shootAllBalls(double targetTime = -1)
{
	updateShooter(true);

	collector->setAutomatic(true);
	if (System::currentTimeMillis() >= targetTime)
	{
		if (!collector->isShooting() && shooter->isStableReady())
		{
			stableCount = 0;
			collector->requestShot();
			hasResetItem = true;
		}
		if (!collector->isCollecting() && !collector->isShooting())
		{
			if (collector->automatic)
			{
				for (int slot = 0; slot<3; slot++)
				{
					if (collector->getSense(slot))
					{
						return false;
					}
				}
				updateShooter(false);
				hasResetItem = false;
				return true;
			} else if (hasResetItem)
			{
				stableCount++;
				if (stableCount > 250)
				{
					updateShooter(false);
					stableCount = 0;
					hasResetItem = false;
					return true;
				}
			}
		}
	}
	return false;
}

#if TRACKING_CAMERA
bool PewPewBot::camYawAlign()
{
	//if (camera->hasData()){
	cout << "Gyro: " << drive->getGyro() << " Error: "
	<< drive->turnPID->GetError() << endl;
	yawAlignState = true;
	if (rotateRobot(270,5))
	{
		yawAlignState = false;
		return true;
	}
	return false;
}
#endif

bool PewPewBot::collectAllBalls()
{
	if (!hasResetItem)
	{
		collector->requestCollect();
		hasResetItem = true;
	}
	if (!collector->isCollecting())
	{
		hasResetItem = false;
		return true;
	}
	return false;
}

bool PewPewBot::rotateRobot(float angle, float tolerance)
{
	if (!hasResetItem)
	{
		drive->resetGyro();
		hasResetItem = true;
		drive->disablePID();
		drive->turnPID->Enable();
		drive->turnPID->SetSetpoint(angle);
	}
	cout << drive->turnPID->GetError() << endl;
	if (fabs(drive->turnPID->GetError()) <= tolerance)
	{
		hasResetItem = false;
		drive->turnPID->Disable();
		drive->enablePID();
		return true;
	}
	return false;
}
