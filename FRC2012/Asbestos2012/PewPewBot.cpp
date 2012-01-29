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

void PewPewBot::OperatorControl() {
	int count = 0;//DEBUG (C1983)
	while (IsOperatorControl() && !IsDisabled()) {
		count++;
		if(count/200 == ((float)count)/200)
		{
			printf("Left: %d Right: %d \n",drive->getL(),drive->getR());
		}
		
		//Set the compressor
		drive->updateCompressor();
		
#if KINECT
		drive->setSpeedR(kinect->getRight());
		drive->setSpeedL(kinect->getLeft());
#else
		//Set the drive base to the stick speeds
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
