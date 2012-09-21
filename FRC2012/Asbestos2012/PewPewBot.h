#ifndef __PEWPEWBOT_H
#define __PEWPEWBOT_H
#include "WPILib.h"
#include <math.h>
#include <fstream>
#include "Utility.h"
#include "1983Defines2012.h"
#include "C1983Drive.h"
#include "C1983Kinect.h"
#include "C1983Collector.h"
#include "C1983Shooter.h"
#if TRACKING_CAMERA
#include "C1983GlyphCamera.h"
#endif
/**
 * @author:
 * 
 */
class PewPewBot : public SimpleRobot
{
public:
	enum AutonomousMode
	{
		kDoYawAlign, kDoDepthAlign, kCollect, kShoot, kRotate180, kMoveToBridge, kTipBridge, kKinect, kDone
	};
	char * getModeName(AutonomousMode mode);
private:
	int stableCount; //This is just used for stablizing things like depth alignment
	AutonomousMode autonomousMode; //The current autonomous mode
	bool hasResetItem;
	bool yawAlignState;
	double PIDAdjust;
	bool flashState;
	double nextFlash;
public:
	C1983Drive *drive;

	C1983Collector *collector;
	C1983Shooter *shooter;
#if TRACKING_CAMERA
	C1983GlyphCamera *camera;
#endif
	//Controls
	DriverStation * driverStation;
	DriverStationLCD * driverStationLCD;
	DriverStationEnhancedIO * myEIO;

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

	void updateDriverStation();
	void updateShooter(bool enabled);

	//Automatic Functions
	bool lineDepthAlign(); //Aligns the robot based on the key
#if TRACKING_CAMERA
	bool camYawAlign(); //Aligns the robot based on the camera data
#endif
	void kinectCode(); //Controls the robot with kinect data

	void cleanPIDs();

	//Autonomous Functions
	bool shootAllBalls(double targetTime);
	bool collectAllBalls();
	bool rotateRobot(float angle, float tolerance);
	bool driveToBridge();
};
#endif
