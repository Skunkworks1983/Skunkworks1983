#include "C1983ShooterPIDSource.h"

C1983ShooterPIDSource::C1983ShooterPIDSource(Encoder * e, double topSpeed,bool reversed)
{
	maxSpeed = topSpeed;
	theEncoder = e;
	reverse = reversed;
	values = new double[(int)AVERAGE_LENGTH];
	for(int i = 0;i < AVERAGE_LENGTH;i++)
	{
		values[i] = 0.0;
	}
	divisonNumber = AVERAGE_LENGTH * (AVERAGE_LENGTH + 1.0)/2.0;
	cout<<"Divison Number: "<<divisonNumber<<endl;
}

void C1983ShooterPIDSource::setMaxSpeed(double newSpeed)
{
	maxSpeed = newSpeed;
}

double C1983ShooterPIDSource::PIDGet()
{
	if(reverse)
	{
		return -average/maxSpeed;
	}else{
		return average/maxSpeed;
	}
}

void C1983ShooterPIDSource::updateAverage()
{
	for(int i = 1;i < AVERAGE_LENGTH;i++)
	{
		//shift the values up the array
		values[i] = values[i-1];
		values[0] = theEncoder->GetRate();
	}
	double temp = 0;
	for(int i = 0;i < AVERAGE_LENGTH;i++)
	{
		temp += values[i] * (AVERAGE_LENGTH - i);
	}
	average = temp/divisonNumber;
}
