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
	case kShoot:
		return "ShootAll";
	case kRotate180:
		return "Rotate180";
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
	int count = 0;

	drive->shift(true);
	drive->resetEncoders();
	autonomousMode = kDoDepthAlign;
	stableCount = 0;

	while (IsAutonomous() && !IsDisabled())
	{
		count++;
		if (count % 25 == 0)
			cout << "Phase: " << getModeName(autonomousMode) << endl;
		drive->updateCompressor();
#if !KINECT
		switch (autonomousMode)
		{
		case kDoYawAlign:
			break;
		case kDoDepthAlign:
			if (lineDepthAlign())
				autonomousMode = kShoot;
			break;
		case kShoot:
			if (shootAllBalls())
				autonomousMode = kRotate180;
			break;
		case kRotate180:
			if (!hasResetItem)
			{
				drive->resetGyro();
				hasResetItem = true;
			}
			if (!drive->turnPID->IsEnabled())
				drive->turnPID->Enable();
			drive->turnPID->SetSetpoint(180);
			if (fabs(drive->turnPID->GetError()) <= 5)
			{
				autonomousMode = kTipBridge;
				drive->turnPID->Disable();
			}
			break;
		case kTipBridge:
			if (!hasResetItem)
			{
				drive->resetEncoders();
				hasResetItem = true;
			}
			drive->tip(true);
			drive->setSpeedL(.25);
			drive->setSpeedR(.25);
			double distance = (drive->getLPosition() + drive->getRPosition())
					/ 2.0;
			if (fabs(distance - 7.0) < 0.5)
			{
				hasResetItem = false;
				autonomousMode = kDone;
			}
			break;
		default:
			//We are done
			break;
		}
#else	
		kinectCode();
#endif
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

	if(kinect->getShootButton())
	{
		collector->jankyGo()
	} else
	{
		collector->jankyStop();
	}

	lastShooterChange--;
	if(kinect->getShooterOnButton() && lastShooterChange < 0)
	{
		lastShooterChange = 30;
		shooter->setOn(shooter->isOn());
	}
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
	if (drive->getLSetpoint() == 0.0)
	drive->resetLeftI();

	if (fabs(drive->getRSetpoint() == 0.0)
			drive->resetRightI();
#endif
		}
#endif
