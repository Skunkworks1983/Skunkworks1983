#ifndef __C1983PIDOutput_H
#define __C1983PIDOutput_H
#include "WPILib.h"

class C1983PIDOutput : public PIDOutput
{
private:
	Victor *vic1;
	Victor *vic2;
	bool reverse;
public:
	C1983PIDOutput(Victor *vic01, Victor *vic02,bool reversed);
	void PIDWrite(float output);
};

#endif
