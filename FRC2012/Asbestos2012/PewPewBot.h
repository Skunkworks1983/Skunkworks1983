#include "WPILib.h"
#include "1983Defines2012.h"
#include "C1983Drive.h"
#include "C1983Kinect.h"
#include "C1983Collector.h"
#include "C1983Shooter.h"
#include <math.h>

/**
 * @author:
 * 
 */
class PewPewBot : public SimpleRobot
{
	//enum representing control states
	enum AutoMode
	{
		kLineDepthAlign, kCameraDepthAlign
	};
public:

	C1983Drive *drive;

	C1983Collector *collector;
	C1983Shooter *shooter;
	//C1983GlyphCamera *camera;

	//Controls
	DriverStation * driverStation;
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

	//Decides automatic state
	int getAutonomousMode();
	int getOperatorControlMode();

	//Automatic Functions
	void automaticMode(int autoMode); //Switches to the correct auto mode

	void lineDepthAlign(); //Aligns the robot based on 'Z'
	void kinectCode(); //Controls the robot with kinect data
};
