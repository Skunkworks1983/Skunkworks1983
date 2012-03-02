#include "PewPewBot.h"

bool PewPewBot::lineDepthAlign()
{
	if (drive->getLightSensorBack() && drive->getLightSensorFront()) //We are on the key, backwards
	{
		drive->setSpeedL(LINE_STOP_SPEED);
		drive->setSpeedR(LINE_STOP_SPEED);
		stableCount = 0;
	} else if (!drive->getLightSensorFront() && !drive->getLightSensorBack())
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

bool PewPewBot::shootAllBalls()
{
	shooter->setShot(C1983Shooter::kKeytop);
	shooter->update();
	collector->update();
	if (!collector->isShooting() && shooter->isReady())
	{
		collector->requestShot();
	}
	if (collector->automatic && !collector->isCollecting()
			&& !collector->isShooting())
	{
		for (int slot = 0; slot<3; slot++)
		{
			if (collector->getSense(slot) == true)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool PewPewBot::camYawAlign()
{
	if (camera->hasData())
	{
		if (!hasResetItem)  //If not made the reset gyro step
		{
			drive->resetGyro();
			hasResetItem = true;
		}
		if (!drive->turnPID->IsEnabled())  //If the turning PID is not enabled
			drive->turnPID->Enable();
		drive->turnPID->SetSetpoint((float)camera->getCurrentYaw());  //Set the turning setpoint
		if (fabs(drive->turnPID->GetError()) <= 5)
		{
			drive->turnPID->Disable();  //Disable and return
			hasResetItem = false;
			return true;
		}
	}
	return false;
}
