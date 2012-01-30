#ifndef __C1983PIDSource_
#define __C1983PIDSource_

#include "WPILib.h"

class C1983PIDSource : public PIDSource
{
private:
	Encoder *theEncoder;
	double maxSpeed;
	bool reverse;
	
public:
	C1983PIDSource(Encoder * e, double topSpeed,bool reversed);
	double PIDGet();
	void setMaxSpeed(double newSpeed);
};
#endif
