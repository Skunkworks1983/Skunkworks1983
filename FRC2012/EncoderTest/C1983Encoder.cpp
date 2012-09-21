#include "C1983Encoder.h"

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
	theNotifier = new Notifier(C1983Encoder::callGetPosition,this);

	theCounter->SetMaxPeriod(1.0);
	theCounter->SetSemiPeriodMode(true);
	theCounter->Reset();
	theCounter->Start();

	m_nCounts = 0;	
	
	data = new double[10];
	timeData = new double[10];
	timediff();

	theNotifier->StartPeriodic(0.005);
}


void C1983Encoder::callGetPosition(void *param)
{
	((C1983Encoder*) param)->getPosition();
}

const double EncoderPulseLength =  1.0 / 2.26;
void C1983Encoder::getPosition()
{
	static int nOutput = 0;
	double fPeriod;
	double fSample;
	
	fPeriod = theCounter->GetPeriod();
	
	fSample  = fPeriod * EncoderPulseLength;
	
	if ((nOutput++ % 100) == 0)
		cout << "encoder test is " << fPeriod << "\n";
	theCounter->Reset();
	
#if 0
	short result;
	for (short bit = 16; bit > 0; bit--)
	{
		result <<= 1;
		result += theEncoder->Get();
		encoderInterrupt->Pulse(PULSE_LENGTH);
		while (encoderInterrupt->IsPulsing())
		{
		}
	}

	cout<<result<<endl;

	for (int i = 1; i <= 10; i++)
	{
		data[i] = data[i - 1];
		data[0] = theEncoder->Get();
		timeData[i] = timeData[i - 1];
		timeData[0] = timediff();
	}
	double tempRate = 0;
	for (int i = 0; i < 10; i++)
	{
		rate += (data[i] * (10 - i)/timeData[i]);
	}
	rate = tempRate / 55;
#endif 
	return;
}

double C1983Encoder::getRate()
{
	return rate;
}
