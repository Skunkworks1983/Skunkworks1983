#include "PewPewBot.h"
#define STOP_SPEED 3
void PewPewBot::Autonomous()
{
	drive->shift(true);
	short hitLine1 = 0, hitLine2 = 0;
	short hitLineF = 0, hitLineB = 0;
	float lineDirection = -2; //-1, backwards, 0, stopped, 1 forwards,  2 = fast, 1 = slow
	int count = 0;
	while (IsAutonomous() && IsEnabled())
	{
		drive->updateCompressor();
#if !KINECT
		count++;
		//if(count/10 == (float)count/10) cout<<"Front Sensor: "<<drive->getLightSensorFront()<<" Back Sensor: "<<drive->getLightSensorBack()<<endl;
		if (drive->getLightSensorBack() == drive->getLightSensorFront())
		{
			drive->setSpeedL(lineDirection);
			dirve->setSpeedR(lineDirection);
		} else if (drive->getLightSensorFront() && !drive->getLightSensorBack())
		{
			hitLineB = 0;
			hitLineF++;
			lineDirection = lineDirection<0 ? -.1 : .1;
			if (hitLineF>=STOP_SPEED)
				lineDirection = 1;
		} else if (!drive->getLightSensorFront()
				&& drive->getLightSensorFront())
		{
			hitLineF = 0;
			hitLineB++;
			lineDirection = lineDirection<0 ? -.1 : .1;
			if (hitLineB>=STOP_SPEED)
				lineDirection = -1;
		}
		/*if (count/10 == (float)count/10)cout<<"Direction: " << lineDirection << endl;
		 if (hitLine1 <= STOP_SPEED && !drive->getLightSensorFront())
		 {
		 drive->setSpeedL(1.0);
		 drive->setSpeedR(1.0);
		 } else if (hitLine1 <= STOP_SPEED && drive->getLightSensorFront())
		 {
		 hitLine1++;
		 drive->setSpeedL(0.2);
		 drive->setSpeedR(0.2);
		 } else if (hitLine2 <= STOP_SPEED && !drive->getLightSensorBack())
		 {
		 drive->setSpeedL(0.2);
		 drive->setSpeedR(0.2);
		 } else if (hitLine2 <= STOP_SPEED && drive->getLightSensorBack())
		 {
		 drive->setSpeedL(-0.1);
		 drive->setSpeedR(-0.1);
		 hitLine2++;
		 } else if (hitLine2 > STOP_SPEED && drive->getLightSensorFront())
		 {
		 drive->setSpeedL(0.05);
		 drive->setSpeedR(0.05);
		 } else if (hitLine2 >STOP_SPEED && drive->getLightSensorBack())
		 {
		 drive->setSpeedL(-0.05);
		 drive->setSpeedR(-0.05);
		 } else
		 {
		 drive->setSpeedL(0.0);
		 drive->setSpeedR(0.0);
		 }*/

#else	
		if(fabs(kinect->getRight())> 0.05)
		{
			drive->setSpeedR(kinect->getRight());
		} else
		{
			drive->setSpeedR(0.0);
		}
		if(fabs(kinect->getLeft())> 0.05)
		{
			drive->setSpeedL(kinect->getLeft());
		} else
		{
			drive->setSpeedL(0.0);
		}

		//Shift Stuff
		if(kinect->getShiftButton())
		{
			drive->shift(true);
		} else
		{
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
