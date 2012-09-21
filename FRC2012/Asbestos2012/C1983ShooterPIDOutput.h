#ifndef __C1983ShooterPIDOutput_H
#define __C1983ShooterPIDOutput_H
#include "WPILib.h"
#include "C1983ShooterPIDSource.h"

class C1983ShooterPIDOutput : public PIDOutput
{
private:
	Victor *vic1;
	Victor *vic2;
	C1983ShooterPIDSource * source;
	bool reverse;
	float setpoint;
	float curr;
public:
	C1983ShooterPIDOutput(Victor *vic01, Victor *vic02,bool reversed,C1983ShooterPIDSource * source);
	void PIDWrite(float output);
	void setSetpoint(float set);
};

#endif
