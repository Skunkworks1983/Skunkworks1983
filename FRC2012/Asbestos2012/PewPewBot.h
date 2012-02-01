#include "WPILib.h"
#include "1983Defines2012.h"
#include "C1983Drive.h"
#include "C1983Kinect.h"
#include <math.h>

/**
 * @author:
 * 
 */
class PewPewBot : public SimpleRobot
{
public:
	C1983Drive *drive;
	
	/*
	C1983Collector collector;
	C1983Shooter shooter;
	C1983Tipper tipper;
	C1983Camera camera;
	*/
	
	//Controls
	Joystick *lStick;
	Joystick *rStick;
#if KINECT
	C1983Kinect *kinect;
#endif
	PewPewBot();
	void Autonomous();
	void OperatorControl();
	void Disabled();
	~PewPewBot();
};
