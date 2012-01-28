#include "PewPewBot.h"

PewPewBot::PewPewBot() {
	drive = new C1983Drive();
#if KINECT
	kinect = new C1983Kinect();
#endif
	lStick = new Joystick(1);
	rStick = new Joystick(2);
}

PewPewBot::~PewPewBot() {
}

void PewPewBot::Autonomous() {
	bool done = false;
	while (IsAutonomous() && IsEnabled()) {
		if (!done) {
			drive->setSpeedL(0.05);
			drive->setSpeedR(0.05);
			Wait(2);
			drive->setSpeedL(0.05);
			drive->setSpeedR(0.05);
			Wait(2);
			drive->setSpeedR(0.0);
			drive->setSpeedL(0.0);
		}
		done = true;
	}

}

void PewPewBot::OperatorControl() {
	while (IsOperatorControl() && !IsDisabled()) {
		
		//Set the compressor
		drive->updateCompressor();
		
#if KINECT
		drive->setSpeedR(kinect->getRight());
		drive->setSpeedL(kinect->getLeft());
#else
		drive->setSpeedR(rStick->GetY());
		drive->setSpeedL(lStick->GetY());
#endif
		
		//Check for shifting
		if(lStick->GetRawButton(1))
		{
			drive->shift(false);
		}else{
			drive->shift(true);
		}
		
	}
}

void PewPewBot::Disabled() {

}

START_ROBOT_CLASS(PewPewBot)
;
