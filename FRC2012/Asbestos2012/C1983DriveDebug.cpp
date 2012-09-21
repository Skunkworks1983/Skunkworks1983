#include "C1983Drive.h"
#if DRIVE_PID
float C1983Drive::getLError()
{
	return (float)((int)(leftPID->GetError() * 100))/100;
}

float C1983Drive::getI()
{
	return leftPID->GetI();
}

float C1983Drive::getP()
{
	return leftPID->GetP();
}

float C1983Drive::getD()
{
	return leftPID->GetD();
}

float C1983Drive::getLSetpoint()
{
	return (float)((int)(leftPID->GetSetpoint() * 100))/100;
}

float C1983Drive::getRSetpoint()
{
	return (float)((int)(rightPID->GetSetpoint() * 100))/100;
}

double C1983Drive::getLPercent()
{
	return leftPIDSource->PIDGet();
}

double C1983Drive::getRPercent()
{
	return rightPIDSource->PIDGet();
}

void C1983Drive::iUp()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI() + .01,rightPID->GetD());
	leftPID->SetPID(leftPID->GetP(),leftPID->GetI() + .01,leftPID->GetD());
}
	
void C1983Drive::iDown()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI() - .01,rightPID->GetD());
	leftPID->SetPID(rightPID->GetP(),leftPID->GetI() - .01,leftPID->GetD());
}

void C1983Drive::pUp()
{
	rightPID->SetPID(rightPID->GetP() + .05,rightPID->GetI(),rightPID->GetD());
	leftPID->SetPID(leftPID->GetP() + .05,leftPID->GetI(),leftPID->GetD());
}
	
void C1983Drive::pDown()
{
	rightPID->SetPID(rightPID->GetP() - .05,rightPID->GetI(),rightPID->GetD());
	leftPID->SetPID(rightPID->GetP() - .05,leftPID->GetI(),leftPID->GetD());
}

void C1983Drive::dUp()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI(),rightPID->GetD() + .05);
	leftPID->SetPID(leftPID->GetP(),leftPID->GetI(),leftPID->GetD() + .05);
}
	
void C1983Drive::dDown()
{
	rightPID->SetPID(rightPID->GetP(),rightPID->GetI(),rightPID->GetD() - .05);
	leftPID->SetPID(rightPID->GetP(),leftPID->GetI(),leftPID->GetD() - .05);
}
#endif
void C1983Drive::debugPrint()
{
	cout<<"Angle: "<<gyro->GetAngle();
	//cout << "P: " << getP() << "\tI: " << getI() << "\tD: " << getD();
	//cout<<"LRate: " << leftEncoder->GetRate() << "\t\t\tRRate: " << rightEncoder->GetRate();
}
