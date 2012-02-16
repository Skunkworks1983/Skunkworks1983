#include "C1983TurnPIDOutput.h"

C1983TurnPIDOutput::C1983TurnPIDOutput(Jaguar *ljag1,Jaguar *ljag2, Jaguar *rjag1, Jaguar *rjag2)
{
	jagLeft1 = ljag1;
	jagLeft2 = ljag2;
	jagRight1 = rjag1;
	jagRight2 = rjag2;
}

void C1983TurnPIDOutput::PIDWrite(float output)
{
	jagLeft1->Set(-output);
	jagLeft2->Set(-output);
	jagRight1->Set(-output);
	jagRight2->Set(-output);
}
