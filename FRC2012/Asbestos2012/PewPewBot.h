#include "WPILib.h"
#include "1983Defines2012.h"
#include "C1983Drive.h"
#include "C1983Kinect.h"
#include "C1983Collector.h"
#include "C1983Shooter.h"
//#include "C1983GlyphCamera.h"
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
		kDoYawAlign, kDoDepthAlign, kCollect, kShoot, kRotate180, kMoveToBridge, kTipBridge, kDone
	};
	char * getModeName(AutonomousMode mode);
private:
	int stableCount; //This is just used for stablizing things like depth alignment
	AutonomousMode autonomousMode; //The current autonomous mode
	bool hasResetItem;
	bool yawAlignState;
	double PIDAdjust;
	bool flashState;
	long nextFlash;
public:
	static double currentTimeMillis();
	
	C1983Drive *drive;

	C1983Collector *collector;
	C1983Shooter *shooter;
	//C1983GlyphCamera *camera;

	//Controls
	DriverStation * driverStation;
	DriverStationLCD * driverStationLCD;
	DriverStationEnhancedIO * myEIO;
	
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

	void updateDriverStation();
	void updateShooter();
	
	//Automatic Functions
	bool lineDepthAlign(); //Aligns the robot based on the key
	bool camYawAlign();  //Aligns the robot based on the camera data
	void kinectCode(); //Controls the robot with kinect data

	void cleanPIDs();
	
	//Autonomous Functions
	bool shootAllBalls(double targetTime);
	bool driveToBridge();
};
