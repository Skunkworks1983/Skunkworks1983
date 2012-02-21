#if 0
#include "C1983Encoder.h"
#include "math.h"
#include <fstream>

double timediff()
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

C1983Encoder::C1983Encoder(UINT32 channel)
{
	theEncoder = new DigitalInput(channel);
	theCounter = new Counter(theEncoder);
	theNotifier = new Notifier(C1983Encoder::callCalcRate,this);

	theCounter->SetMaxPeriod(1.0);
	theCounter->SetSemiPeriodMode(true);
	theCounter->Reset();
	theCounter->Start();

	m_nCounts = 0;

	data = new double[20];
	theNotifier->StartPeriodic(ENCODER_SAMPLE_RATE);
}

void C1983Encoder::callCalcRate(void *param)
{
	((C1983Encoder*) param)->calcRate();
}

bool lastEncState = false;
void C1983Encoder::calcRate()
{
	static int nOutput = 0;
	if (theCounter->GetPeriod() <= (ENCODER_MAX/2))
	{
		if (lastEncState)
		{
			thisRate = ((double)60) / timediff();
			for (int i = 1; i < 20; i++)
			{
				data[i] = data[i - 1];
			}
			data[0] = thisRate;
			double tempRate = 0;
			for (int i = 0; i < 20; i++)
			{
				tempRate += data[i] * (20 - i);
			}
			rate = tempRate / (double)210.0;
		}
		lastEncState = false;
	} else
	{
		lastEncState = true;
	}
	/*fSample = getAngle(theCounter->GetPeriod());
	 thisRate = angleDifference(fPrevSample, fSample)
	 /(double)(ENCODER_SAMPLE_RATE * 6.0);
	 */
	if ((nOutput++ % 87) == 0)
	{
		cout << "This Rate: " << thisRate << "\tRate: "<<rate <<endl;
	}
	//fPrevSample = fSample;
	theCounter->Reset();
	/*if (fabs(thisRate - rate) > 200)
	 {
	 return;
	 }*/
	return;
}

double C1983Encoder::getAngle(double input)
{
	if (input > ENCODER_MAX)
	{
		input = ENCODER_MAX;
	}
	return input * ENCODER_TO_ANGLE;}

	//angle1 was found first
double C1983Encoder::angleDifference(double angle1, double angle2)
{
	double preCalc = angle1 - angle2;
	if (preCalc >= 0)
	{
		return preCalc;
	} else
	{
		return preCalc + 360;
	}
}
#endif
