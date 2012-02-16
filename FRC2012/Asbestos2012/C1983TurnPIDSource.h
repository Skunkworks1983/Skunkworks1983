#include "WPILib.h"

class C1983TurnPIDSource : public PIDSource
{
public:
	C1983TurnPIDSource(Gyro *gyro);
	
	double PIDGet();
private:
	Gyro *theGyro;
};
