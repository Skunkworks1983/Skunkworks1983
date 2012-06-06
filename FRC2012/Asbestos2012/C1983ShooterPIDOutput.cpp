#include "C1983ShooterPIDOutput.h"

C1983ShooterPIDOutput::C1983ShooterPIDOutput(Victor *vic01, Victor *vic02,
		bool reversed, C1983ShooterPIDSource * theSource)
{
	vic1 = vic01;
	vic2 = vic02;
	reverse = reversed;
	source = theSource;
	setpoint = 0.0;
	curr = 0.0;
}

void C1983ShooterPIDOutput::setSetpoint(float set)
{
	setpoint = set;
}

void C1983ShooterPIDOutput::PIDWrite(float output)
{
	/*
	 if(reverse)
	 {
	 vic1->Set(-output);
	 vic2->Set(-output);
	 }else{
	 vic1->Set(output);
	 vic2->Set(output);
	 }
	 */
	source->updateAverage();
#if SHOOTER_CHANGE_LIMIT_IS_ONE
	if (output != 0.0 && source->PIDGet() < setpoint)
	{
		curr = 1;
	else
	{
		curr = 0;
	}
#else
	double speed = source->PIDGet();
	if (speed >= setpoint && output != 0.0)
	{
		curr = SPINDOWN_SPEED;
	}else if(speed >= GO_POINT && output != 0.0){
		curr = GO_SPEED;
	}else if (speed >= SPINUP_LOW_POINT && output != 0.0){
		curr = SPINUP_SPEED;
	}else if (speed >= SCALE_LOW_POINT && output != 0.0){
		curr = speed * SHOOTER_MAX_SPEED * 0.001; //this makes the power 10% when at 100 RPM, 30% when at 300 etc
	}else if(output != 0.0){
		curr = BEGIN_SPEED;
	}else{
		curr = 0;
	}
#endif
	if (reverse)
	{
		vic1->Set(-curr);
		vic2->Set(-curr);
	} else
	{
		vic1->Set(curr);
		vic2->Set(curr);
	}
}
