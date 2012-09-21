#include "C1983PIDSource.h"

C1983PIDSource::C1983PIDSource(Encoder * e, double topSpeed,bool reversed)
{
	maxSpeed = topSpeed;
	theEncoder = e;
	reverse = reversed;
}

void C1983PIDSource::setMaxSpeed(double newSpeed)
{
	maxSpeed = newSpeed;
}

double C1983PIDSource::PIDGet()
{
	if(reverse)
	{
		return -theEncoder->GetRate()/maxSpeed;
	}else{
		return theEncoder->GetRate()/maxSpeed;
	}
}
