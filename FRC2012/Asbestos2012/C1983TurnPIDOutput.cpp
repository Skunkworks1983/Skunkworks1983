#include "C1983TurnPIDOutput.h"

C1983TurnPIDOutput::C1983TurnPIDOutput(Victor *lvic1,Victor *lvic2, Victor *rvic1, Victor *rvic2)
{
	vicLeft1 = lvic1;
	vicLeft2 = lvic2;
	vicRight1 = rvic1;
	vicRight2 = rvic2;
}

//Gyro increases as you rotate CCW
void C1983TurnPIDOutput::PIDWrite(float output)
{
	vicLeft1->Set(-output);
	vicLeft2->Set(-output);
	vicRight1->Set(-output);
	vicRight2->Set(-output);
}
