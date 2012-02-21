#if 0
#ifndef __C1983Encoder_H
#define __C1983Encoder_H
#include "WPILib.h"
#include "1983Defines2012.h"

#define PULSE_LENGTH 0.0001 

class C1983Encoder
{
public:
	C1983Encoder(UINT32 channel);
	
	static void callCalcRate(void * encoder);
	
	double getRate();
	
	double getAngle(double input);
	double angleDifference(double angle1, double angle2);
private:
	void calcRate();
	
	DigitalInput * theEncoder;
	Notifier *theNotifier;
	Counter *theCounter;
	
	int		m_nCounts;
	double rate;
	double thisRate;
	double *data;
	double fPrevSample;
	double fSample;
};
#endif
#endif
