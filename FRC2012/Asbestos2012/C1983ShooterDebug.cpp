#include "C1983Shooter.h"
#if SHOOTER_PID
float C1983Shooter::getError()
{
	return (float)((int)(shooterPID->GetError() * 100))/100;
}

float C1983Shooter::getI()
{
	return shooterPID->GetI();
}

float C1983Shooter::getP()
{
	return shooterPID->GetP();
}

float C1983Shooter::getD()
{
	return shooterPID->GetD();
}

float C1983Shooter::getSetpoint()
{
	return (float)((int)(shooterPID->GetSetpoint() * 100))/100;
}

double C1983Shooter::getPercent()
{
	return shooterPIDSource->PIDGet();
}

void C1983Shooter::iUp()
{
	shooterPID->SetPID(shooterPID->GetP(),shooterPID->GetI() + .01,shooterPID->GetD());
}
	
void C1983Shooter::iDown()
{
	shooterPID->SetPID(shooterPID->GetP(),shooterPID->GetI() - .01,shooterPID->GetD());
}

void C1983Shooter::pUp()
{
	shooterPID->SetPID(shooterPID->GetP() + .01,shooterPID->GetI(),shooterPID->GetD());
}
	
void C1983Shooter::pDown()
{
	shooterPID->SetPID(shooterPID->GetP() - .01,shooterPID->GetI(),shooterPID->GetD());
}

void C1983Shooter::dUp()
{
	shooterPID->SetPID(shooterPID->GetP(),shooterPID->GetI(),shooterPID->GetD() + .05);
}
	
void C1983Shooter::dDown()
{
	shooterPID->SetPID(shooterPID->GetP(),shooterPID->GetI(),shooterPID->GetD() - .05);
}
#endif

void C1983Shooter::debugPrint()
{
#if SHOOTER_PID
	//cout<<"P: "<<getP()<<" Setpoint: "<<getSetpoint()<<" Value: "<<shooterEncoder->GetRate()<<endl;
	cout<<"Rate: "<<shooterEncoder->GetRate()<<" PID Get: "<<shooterPIDSource->PIDGet()<<" P: "<<getP();
#else
	cout<<"Rate: "<<average;
#endif
}

double C1983Shooter::getRate()
{
	return average;
}

void C1983Shooter::openFile()
{
	data.open("shooterData.csv");
	cout<<"FILE OPEN"<<endl;
	data<<"Index,Rate\n";
}

void C1983Shooter::closeFile()
{
	cout<<"CLOSING FILE"<<endl;
	data.close();
}
