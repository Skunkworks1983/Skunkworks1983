#include "C1983ShooterPIDSource.h"

double timediffZ()
{
	static struct timespec ts =
	{ 0, 0 };
	struct timespec tt;
	clock_gettime(CLOCK_REALTIME, &tt);
	double diff = ((double) tt.tv_sec - ts.tv_sec) + (tt.tv_nsec - ts.tv_nsec)
			/1000000000.0;
	clock_gettime(CLOCK_REALTIME, &ts);
	return diff;
}


C1983ShooterPIDSource::C1983ShooterPIDSource(Encoder * e, double topSpeed,bool reversed)
{
	maxSpeed = topSpeed;
	theEncoder = e;
	reverse = reversed;
	values = new double[(int)AVERAGE_LENGTH];
	times = new double [(int)AVERAGE_LENGTH];
	average = 0;
	lastPos = 0.0;
	for(int i = 0;i < AVERAGE_LENGTH;i++)
	{
		values[i] = 0.0;
		times[i] = 1.0;
	}
	timediffZ();
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
		times[0] = timediffZ();
		double position = theEncoder->GetDistance();
		average = (lastPos - position)/times[0];
		lastPos = position;

		for (int i = ((int)AVERAGE_LENGTH) - 1; i > 0; i--)
		{
			//shift the values up the array
			values[i] = values[i-1];
			times[i] = times[i-1];
		}
		values[0] = position;
		double temp = 0;
		for (int i = 1; i < AVERAGE_LENGTH; i++)
		{
			temp += (values[i] - values[i - 1])/times[i];// * (AVERAGE_LENGTH - i);
		}
		realAverage = temp/((float)(AVERAGE_LENGTH - 1));
}

double C1983ShooterPIDSource::getRealAverage()
{
	if(reverse)
	{
		return -realAverage/maxSpeed;
	}else{
		return realAverage/maxSpeed;
	}
}
