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
	case kKinect:
		return "Kinect";
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

	drive->shift(false);
	drive->resetEncoders();
	autonomousMode = kCollect;
	stableCount = 0;

	//Cleaning
	collector->clean();
	
	double startTime = PewPewBot::currentTimeMillis();
	
	//Spinup shooter
	shooter->setEnabled(true);
	shooter->setShot(AUTONOMOUS_SHOT);
	
	while (IsAutonomous() && !IsDisabled())
	{
#if KINECT
		if (kinect->getKinectMode())
		{
			autonomousMode = kKinect;
		}
#endif
		
		drive->updateCompressor();
		shooter->update();
		collector->update();
		updateDriverStation();
		
		switch (autonomousMode)
		{
		case kDoYawAlign:
			break;
		case kDoDepthAlign:
			if (lineDepthAlign())
				autonomousMode = kCollect;
			break;
		case kCollect:
			if (collectAllBalls())
				autonomousMode = kShoot;
			break;
		case kShoot:
			if (shootAllBalls(AUTONOMOUS_DELAY_SWITCH?startTime + AUTONOMOUS_DELAY:-1))
				autonomousMode = AUTONOMOUS_FULL_AUTO_SWITCH ? kRotate180
						: kKinect;
			break;
		case kRotate180:
			if (rotateRobot(180))
				autonomousMode = kMoveToBridge;
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
		case kKinect:
#if KINECT
			kinectCode();
#else
			autonomousMode = kDone;
#endif
			break;
		default:
			//We are done
#if SHOOTER_PID
			shooter->setEnabled(false);
#endif
			break;
		}
		GetWatchdog().Feed();
		Wait(0.02);
	}

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
	/*
	static int lastShooterChange = 30;
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
