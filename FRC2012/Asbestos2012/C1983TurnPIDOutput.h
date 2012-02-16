#include "WPILib.h"

class C1983TurnPIDOutput : public PIDOutput
{
public:
	C1983TurnPIDOutput(Jaguar *ljag1, Jaguar *ljag2, Jaguar *rjag1, Jaguar *rjag2);
	void PIDWrite(float output);
	
private:
	Jaguar *jagLeft1,*jagLeft2,*jagRight1,*jagRight2;
};
