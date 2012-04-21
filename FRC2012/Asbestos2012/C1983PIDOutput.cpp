#include "C1983PIDOutput.h"

C1983PIDOutput::C1983PIDOutput(Victor *vic01, Victor *vic02,bool reversed)
{
	vic1 = vic01;
	vic2 = vic02;
	reverse = reversed;
}

void C1983PIDOutput::PIDWrite(float output)
{
	if(reverse)
	{
#if !(GOGO)
		vic1->Set(-output);
#endif
		vic2->Set(-output);
	}else{
#if !(GOGO)
		vic1->Set(output);
#endif
		vic2->Set(output);
	}
}
