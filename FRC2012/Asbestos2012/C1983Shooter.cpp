#include "C1983Shooter.h"
#include "Victor.h"

C1983Shooter::C1983Shooter()
{
	isRunning = false;
}

void C1983Shooter::shoot()
{

}

void C1983Shooter::setVelocity(float velocity)
{

}

float C1983Shooter::getVelocity()
{
	return 0;
}

bool C1983Shooter::isReady()
{
	float diff = getVelocity() - TARGET_SHOOT_VELOCITY;
	return diff >= -SHOOT_VELOCITY_TOLERANCE && diff<=SHOOT_VELOCITY_TOLERANCE;
}

