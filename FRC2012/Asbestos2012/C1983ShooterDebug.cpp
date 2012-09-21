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
#endif
float C1983Shooter::getSetpoint()
{
	return shooterPID->GetSetpoint() * SHOOTER_MAX_SPEED;
}
double C1983Shooter::getPercent()
{
	return shooterPIDSource->PIDGet();
}

void C1983Shooter::iUp()
{
	switch (currentShot)
	{
	case kFreethrow:
		iLow += .001;
		break;
	case kKeytop:
		iHigh += .001;
		break;
	}
}

void C1983Shooter::iDown()
{
	switch (currentShot)
	{
	case kFreethrow:
		iLow -= .001;
		break;
	case kKeytop:
		iHigh -= .001;
		break;
	}
}

void C1983Shooter::pUp()
{
	cout<<"P up"<<endl;
	switch (currentShot)
	{
	case kFreethrow:
		pLow += .05;
		break;
	case kKeytop:
		pHigh += .05;
		break;
	}
}

void C1983Shooter::pDown()
{
	switch (currentShot)
	{
	case kFreethrow:
		pLow -= .05;
		break;
	case kKeytop:
		pHigh -= .05;
		break;
	}
}

void C1983Shooter::dUp()
{
	switch (currentShot)
	{
	case kFreethrow:
		dLow += .05;
		break;
	case kKeytop:
		dHigh += .05;
		break;
	}
}

void C1983Shooter::dDown()
{
	switch (currentShot)
	{
	case kFreethrow:
		dLow -= .05;
		break;
	case kKeytop:
		dHigh -= .05;
		break;
	}
}
/*
 void C1983Shooter::debugPrint()
 {
 #if SHOOTER_PID
 //cout<<"P: "<<getP()<<" Setpoint: "<<getSetpoint()<<" Value: "<<shooterEncoder->GetRate()<<endl;
 //cout<<" Setpoint: "<<shooterPID->GetSetpoint() * SHOOTER_MAX_SPEED<<" Rate: "<<shooterPIDSource->PIDGet() * SHOOTER_MAX_SPEED<<" Error: "<<shooterPID->GetError() * SHOOTER_MAX_SPEED<<" I: "<<getI()<<" ShooterReady: "<<isReady();

 #else
 cout<<"Rate: "<<shooterEncoder->GetRate();
 #endif
 }
 */
double C1983Shooter::getRate()
{
#if SHOOTER_PID
	return shooterPIDSource->PIDGet();
#else
	return shooterEncoder->GetRate();
#endif
}

void C1983Shooter::openFile()
{
	data = new ofstream();
	cout<<"File Open"<<endl;
	data->open(getFileName());
	(*data)<<"Index,VICTOR,Rate,Setpoint,P: \n";//<<shooterPID->GetP()<<",I: "
	//<<shooterPID->GetI()<<",D: "<<shooterPID->GetD()<<"\n";
	fileOpen = true;
}

void C1983Shooter::closeFile()
{
	cout<<"File Close"<<endl;
	fileNumber++;
	fileIndex = 0;
	data->close();
	delete data;
	fileOpen = false;
}

bool C1983Shooter::getIsOpen()
{
	return fileOpen;
}

void C1983Shooter::writeFile()
{
	(*data)<<(float)fileIndex/50.0<<","<<shooterVic1->Get() << ","
			<< shooterPIDSource->getRealAverage() * SHOOTER_MAX_SPEED<<","
			<<shooterPID->GetSetpoint() * SHOOTER_MAX_SPEED<<",\n";
	//(*data)<<(float)fileIndex/50.0<<","<<shooterVic1->Get()<<","<<shooterPID->GetSetpoint() * SHOOTER_MAX_SPEED<<",\n";
	fileIndex++;
}

char* C1983Shooter::getFileName()
{
	stringstream ss;
	ss << fileNumber;
	string bleh = "shooterData" + ss.str() + ".csv";
	char* output = new char[bleh.size() + 1];
	for (int i = 0; i < (int)bleh.size(); i++)
	{
		output[i] = bleh[i];
		cout<<output[i];
	}
	output[bleh.size()] = 0;
	cout<<endl;
	return output;
}
