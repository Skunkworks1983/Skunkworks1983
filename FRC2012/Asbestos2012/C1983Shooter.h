#ifndef __C1983Shooter_H
#define __C1983Shooter_H
#include "WPILib.h"
#include "math.h"
#include "1983Defines2012.h"
#include "C1983PIDOutput.h"
#include "C1983Encoder.h"
#if SHOOTER
class C1983Shooter
{
private:
	Victor *shooterVic1;
	Victor *shooterVic2;
	Relay *hoodAngler;
	C1983PIDOutput *shooterPIDOutput;
	PIDController *shooterPID;
	//C1983Encoder *shooterEncoder;
	
	float goalRPM;
	bool angleHigh;
	bool active;
	float power;
	
public:
	C1983Shooter();
	bool isReady();
	void setOn(bool on);
	void setPower(float powerRPM);
	void setAngle(bool high);
	void setShot(short shotNum);
	void setJankyPower(float power);
	void jankyStop();
	
	enum {kLayup, kFreethrow, kOther} shot;
};
#endif
#endif
