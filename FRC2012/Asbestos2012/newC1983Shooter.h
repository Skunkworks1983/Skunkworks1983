#if SHOOTER
#ifndef __C1983Shooter_H
#define __C1983Shooter_H
#include "WPILib.h"
#include "math.h"
#include "1983Defines2012.h"

class C1983Shooter
{
private:
	Encoder *shooterEncoder;
	Victor *shooterVic;
	Relay *hoodAngler;
	PIDController *shooterPID;
	
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
	
	enum {kLayup, kFreethrow, kOther} shot;
};
#endif
#endif