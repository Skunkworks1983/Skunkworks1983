#ifndef __C1983ShooterPIDSource_H
#define __C1983ShooterPIDSource_H

#include "WPILib.h"
#include "1983Defines2012.h"

class C1983ShooterPIDSource : public PIDSource
{
private:
	Encoder *theEncoder;
	double maxSpeed;
	double average;
	double realAverage;
	double *values;
	double *times;
	bool reverse;
	double divisonNumber;
	double lastPos;
	
public:
	C1983ShooterPIDSource(Encoder * e, double topSpeed,bool reversed);
	double PIDGet();
	void setMaxSpeed(double newSpeed);
	double getRealAverage();
	void updateAverage();
};
#endif
