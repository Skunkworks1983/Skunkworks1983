#include "PewPewBot.h"
#define GYROTEST 1
char * PewPewBot::getModeName(AutonomousMode mode)
{
	switch (mode)
	{
	case kDoYawAlign:
		return "DoYawAlign";
	case kDoDepthAlign:
		return "DoDepthAlign";
	case kCollect:
		return "Collecting";
	case kShoot:
		return "ShootAll";
	case kRotate180:
		return "Rotate180";
	case kMoveToBridge:
		return "MoveToBridge";
	case kTipBridge:
		return "TipBridge";
	case kDone:
		return "Complete";
	default:
		return "BadMode";
	}
}
void PewPewBot::Autonomous()
{
	GetWatchdog().SetEnabled(true);
	GetWatchdog().SetExpiration(0.2);
	shooter->setEnabled(true);

	drive->shift(false);
	drive->resetEncoders();
	autonomousMode = kCollect;
	stableCount = 0;

	//Cleaning
	collector->clean();

	double startTime = PewPewBot::currentTimeMillis();

	while (IsAutonomous() && !IsDisabled())
	{
#if SHOOTER_PID
		shooter->setEnabled(true);  //Spinup the shooter before running the collecting cycle.
#endif
		drive->updateCompressor();
		updateDriverStation();
		if (KINECT && !kinect->getDoAutonomous())
		{
			kinectCode();
		} else
		{
			switch (autonomousMode)
			{
			case kDoYawAlign:
				break;
			case kDoDepthAlign:
				if (lineDepthAlign())
					autonomousMode = kShoot;
				break;
			case kCollect:
				if (!hasResetItem)
				{
					collector->requestCollect();
					hasResetItem = true;
				}
				if (!collector->isCollecting())
				{
					hasResetItem = false;
					autonomousMode = kShoot;
				}
			case kShoot:
				if (shootAllBalls(AUTONOMOUS_DELAY_SWITCH?startTime + AUTONOMOUS_DELAY:-1))
					autonomousMode = kDone;//kRotate180;
				break;
			case kRotate180:
				if (!hasResetItem)
				{
					drive->resetGyro();
					hasResetItem = true;
					drive->disablePID();
					drive->turnPID->Enable();
					drive->turnPID->SetSetpoint(180);
				}
				//cout << "Get: " << drive->turnPID->Get() << "\tSetpoint: " << drive->turnPID->GetSetpoint() << endl;
				//cout << "Error: " << drive->turnPID->GetError() << endl;
				if (fabs(drive->turnPID->GetError()) <= 5)
				{
					hasResetItem = false;
					autonomousMode = kMoveToBridge;
					drive->turnPID->Disable();
					drive->enablePID();
				}
				break;
			case kMoveToBridge:
				if (!hasResetItem)
				{
					drive->resetEncoders();
					hasResetItem = true;
				}
				drive->setSpeedL(.25);
				drive->setSpeedR(.25);
				double distance = (drive->getLPosition()
						+ drive->getRPosition()) / 2.0;
				if (fabs(distance - 7.0) < 0.5)
				{
					hasResetItem = false;
					autonomousMode = kTipBridge;
				}
				break;
			case kTipBridge:
				drive->tip(true);
				autonomousMode = kDone;
				break;
			default:
				//We are done
#if SHOOTER_PID
				shooter->setEnabled(false);
#endif
				break;
			}
		}
		GetWatchdog().Feed();
		Wait(0.02);
	}

}

#if KINECT
void PewPewBot::kinectCode()
{
	static int lastShooterChange = 30;
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
	/*
	 if(kinect->getShootButton())
	 {
	 collector->jankyGo();
	 } else
	 {
	 collector->jankyStop();
	 }

	 lastShooterChange--;
	 if(kinect->getShooterOnButton() && lastShooterChange < 0)
	 {
	 lastShooterChange = 60;
	 shooter->setEnabled(!shooter->getEnabled());
	 }
	 shooter->setShot(C1983Shooter::kFreethrow);*/
	/*
	 //Shift Stuff
	 if (kinect->getShiftButton())
	 {
	 drive->shift(true);
	 } else
	 {
	 drive->shift(false);
	 }
	 */
#if DRIVE_PID
	//Check whether we're close enough to the setpoint. If so, reset I.
	if (fabs(drive->getLSetpoint()) < 0.03)
	drive->resetLeftI();

	if (fabs(drive->getRSetpoint()) < 0.03)
	drive->resetRightI();
#endif
}
#endif
