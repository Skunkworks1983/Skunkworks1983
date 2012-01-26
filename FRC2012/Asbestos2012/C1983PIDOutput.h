#ifndef __C1983PIDOutput_H
#define __C1983PIDOutput_H
#include "WPILib.h"

class C1983PIDOutput : public PIDOutput
{
private:
	Jaguar *jag1;
	Jaguar *jag2;
public:
	C1983PIDOutput(Jaguar *jag01, Jaguar *jag02);
	void PIDWrite(float output);
};

#endif
