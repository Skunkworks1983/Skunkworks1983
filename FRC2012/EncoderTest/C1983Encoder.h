#ifndef __C1983Encoder_H
#define __C1983Encoder_H
#include "WPILib.h"

#define PULSE_LENGTH 0.0001 

class C1983Encoder
{
public:
	C1983Encoder(UINT32 channel);
	
	static void callGetPosition(void * encoder);
	
	void getPosition();
	
	double getRate();
private:
	DigitalInput * theEncoder;
	Notifier *theNotifier;
	Counter *theCounter;
	
	int		m_nCounts;
	double * data;
	double * timeData;
	double rate;
	int tid;
	
};
#endif
