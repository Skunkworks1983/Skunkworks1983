#include "C1983Drive.h"

double C1983Drive::getL()
{
	return (float)((int)(leftEncoder->GetRate() * 100))/100;
}

double C1983Drive::getR()
{
	return (float)((int)(rightEncoder->GetRate() * 100))/100;
}

double C1983Drive::getLPosition()
{
	return leftEncoder->GetDistance();
}

double C1983Drive::getRPosition()
{
	return rightEncoder->GetDistance();
}

void C1983Drive::resetEncoders()
{
	leftEncoder->Reset();
	rightEncoder->Reset();
}
#if DRIVE_PID
void C1983Drive::resetLeftI()
{
	leftPID->Reset();
	leftPID->Enable();
}

void C1983Drive::resetRightI()
{
	rightPID->Reset();
	rightPID->Enable();
}

void C1983Drive::cleanPID()
{
	leftPID->Reset();
	rightPID->Reset();
}

void C1983Drive::enablePID()
{
	leftPID->Enable();
	rightPID->Enable();
}

void C1983Drive::disablePID()
{
	leftPID->Disable();
	rightPID->Disable();
}
#endif
