#include "PewPewBot.h"
void PewPewBot::Autonomous() {
	int count = 0;
	drive->shift(true);
	while (IsAutonomous() && IsEnabled()) 
	{
		drive->updateCompressor();
#if !KINECT
		if(!drive->getLightSensorFront() && !drive->getLightSensorBack()){
			drive->setSpeedL(-1.0);
			drive->setSpeedR(-1.0);
		}else{
			drive->setSpeedL(0.0);
			drive->setSpeedR(0.0);
		}
		count++;
		if(count/50 == (float)count/50) cout<<"Front Sensor: "<<drive->getLightSensorFront()<<" Back Sensor: "<<drive->getLightSensorBack()<<endl;
		
#else	
	if(fabs(kinect->getRight()) > 0.05)
	{
		drive->setSpeedR(kinect->getRight());
	}else{
		drive->setSpeedR(0.0);
	}
	if(fabs(kinect->getLeft()) > 0.05)
	{
		drive->setSpeedL(kinect->getLeft());
	}else{
		drive->setSpeedL(0.0);
	}
	
	//Shift Stuff
	if(kinect->getShiftButton())
	{
		drive->shift(true);
	}else{
		drive->shift(false);
	}
	
	//Check whether we're close enough to the setpoint. If so, reset I.
	if (fabs(drive->getLSetpoint() - drive->getL()) <= .01)
		drive->resetLeftI();

	if (fabs(drive->getRSetpoint() - drive->getR()) <= .01)
		drive->resetRightI();
#endif
	}
}
