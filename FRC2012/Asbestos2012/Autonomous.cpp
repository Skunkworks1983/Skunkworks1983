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
	int count = 0;
	shooter->setEnabled(true);

	drive->shift(false);
	drive->resetEncoders();
	autonomousMode = kShoot;
	stableCount = 0;

	while (IsAutonomous() && !IsDisabled())
	{
		count++;
		if (count % 25 == 0)
		{
			//shooter->debugPrint();
			//cout<<endl;
			cout << "Phase: " << getModeName(autonomousMode) << endl;
		}
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
				drive->disablePID();
				drive->turnPID->Enable();
				drive->turnPID->SetSetpoint(180);
			}
			cout << "Get: " << drive->turnPID->Get() << "\tSetpoint: " << drive->turnPID->GetSetpoint() << endl;
			cout << "Error: " << drive->turnPID->GetError() << endl;
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
			double distance = (drive->getLPosition() + drive->getRPosition())
					/ 2.0;
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
			shooter->setPower(0.0);
			break;
		}
#else	
		kinectCode();
#endif
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
	shooter->setShot(C1983Shooter::kFreethrow);
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
