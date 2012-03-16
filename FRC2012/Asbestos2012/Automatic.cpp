#include "PewPewBot.h"

bool PewPewBot::lineDepthAlign()
{
	if (drive->getLightSensorBack() && drive->getLightSensorFront()) //We are on the key, forwards
	{
		drive->setSpeedL(LINE_STOP_SPEED);
		drive->setSpeedR(LINE_STOP_SPEED);
		stableCount = 0;
	} else if (!drive->getLightSensorFront() && !drive->getLightSensorBack())  //We are off the key, backwards
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
	shooter->setShot(AUTONOMOUS_SHOT);
#if SHOOTER_PID
	if (PID_SLIDER> .2095)
	{
		shooter->setPIDAdjust(0.0762 * log(PID_SLIDER) + 0.0407);
	} else
	{
		shooter->setPIDAdjust(-0.15);
	}
#endif
	shooter->setEnabled(true);
	collector->setAutomatic(true);
	if (PewPewBot::currentTimeMillis() >= targetTime)
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
				shooter->setEnabled(false);
				hasResetItem = false;
				return true;
			} else if (hasResetItem)
			{
				stableCount++;
				if (stableCount > 250)
				{
					shooter->setEnabled(false);
					stableCount = 0;
					hasResetItem = false;
					return true;
				}
			}
		}
	}
	return false;
}

bool PewPewBot::camYawAlign()
{
	//if (camera->hasData()){
	cout << "Gyro: " << drive->getGyro() << " Error: "
			<< drive->turnPID->GetError() << endl;
	if (drive->turnPID->IsEnabled() && fabs(drive->turnPID->GetError()) <= 0.05)
	{
		cout << "Ending turn cycle" << endl;
		yawAlignState = false;
		drive->turnPID->Disable(); //Disable and return
		drive->enablePID();
		hasResetItem = false;
		return true;
	}
	if (!hasResetItem) //If not made the reset gyro step
	{
		cout << "Beginning turn cycle" << endl;
		yawAlignState = true;
		drive->resetGyro();
		hasResetItem = true;
		drive->disablePID();
		drive->turnPID->Reset();
		drive->turnPID->SetSetpoint((float)270/*-camera->getCurrentYaw()*/); //Set the turning setpoint
		drive->turnPID->Enable();
	}
	//}
	return false;
}

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

bool PewPewBot::rotateRobot(float angle)
{
	if (!hasResetItem)
	{
		drive->resetGyro();
		hasResetItem = true;
		drive->disablePID();
		drive->turnPID->Enable();
		drive->turnPID->SetSetpoint(angle);
	}
	if (fabs(drive->turnPID->GetError()) <= 5)
	{
		hasResetItem = false;
		drive->turnPID->Disable();
		drive->enablePID();
		return true;
	}
	return false;
}
