#include "C1983Shooter.h"
#include "Victor.h"

C1983Shooter::C1983Shooter()
{
shooterVic = new Victor(SHOOTER_WHEEL);
hoodVic = new Victor(SHOOTER_HOOD);
	//isWheelRunning = false;
	//TODO: WTF?
}


void C1983Shooter::setVelocity(float velocity)
{
	targetSpeed = velocity;
	shooterVic->Set(targetSpeed);
}

float C1983Shooter::getVelocity()
{
	return 0;
}

bool C1983Shooter::isReady()
{
	float diff = abs(getVelocity() - targetSpeed);
	float otherDiff= abs(getAngle()- targetAngle);
	return diff >= -SHOOT_VELOCITY_TOLERANCE && otherDiff>=SHOOT_ANGLE_TOLERANCE;
}

void C1983Shooter::angle(float angle)
{
	targetAngle = angle;
	hoodVic->Set(targetAngle);
}
//TOD0: Set Presets and define in defines.h
void  C1983Shooter::aim(int preset)
{
	float preset0Speed=0.0;
	float preset0Angle=0.0;
	float preset1Speed=0.0;
	float preset1Angle=0.0;
	float preset2Speed=0.0;
	float preset2Angle=0.0;
	
	if (preset==0)
		{
			shooterVic->Set(preset0Speed);
			hoodVic->Set(preset0Angle);
		}
	if (preset==1)
		{
			shooterVic->Set(preset1Speed);
			hoodVic->Set(preset1Angle);
		}
	if (preset==2)
		{
			shooterVic->Set(preset2Speed);
			hoodVic->Set(preset2Angle);
		}
}
