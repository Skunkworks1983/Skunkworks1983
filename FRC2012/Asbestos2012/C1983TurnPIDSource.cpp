#include "C1983TurnPIDSource.h"

C1983TurnPIDSource::C1983TurnPIDSource(Gyro *gyro)
{
	theGyro = gyro;
}

double C1983TurnPIDSource::PIDGet()
{
	return theGyro->GetAngle();
}
