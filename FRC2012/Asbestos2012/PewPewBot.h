#include "WPILib.h"
#include "1983Defines2012.h"
#include "C1983Drive.h"
#include "C1983Kinect.h"
#include "C1983Collector.h"
#include "C1983Shooter.h"
#include "C1983GlyphCamera.h"
#include <math.h>
#include <fstream>

/**
 * @author:
 * 
 */
class PewPewBot : public SimpleRobot
{
public:
	enum AutonomousMode
	{
		kDoYawAlign, kDoDepthAlign, kShoot, kRotate180, kTipBridge, kDone
	};
	char * getModeName(AutonomousMode mode);
private:
	int stableCount; //This is just used for stablizing things like depth alignment
	AutonomousMode autonomousMode; //The current autonomous mode
	bool hasResetItem;
public:
	C1983Drive *drive;

	C1983Collector *collector;
	C1983Shooter *shooter;
	C1983GlyphCamera *camera;

	//Controls
	DriverStation * driverStation;
	DriverStationLCD * driverStationLCD;
	Joystick *lStick;
	Joystick *rStick;
	ofstream myfile;
	
#if KINECT
	C1983Kinect *kinect;
#endif
	PewPewBot();
	void Autonomous();
	void OperatorControl();
	void Disabled();
	~PewPewBot();

	//Automatic Functions
	bool lineDepthAlign(); //Aligns the robot based on the key
	bool camYawAlign();  //Aligns the robot based on the camera data
	void kinectCode(); //Controls the robot with kinect data

	//Autonomous Functions
	bool shootAllBalls();
	bool driveToBridge();
};
