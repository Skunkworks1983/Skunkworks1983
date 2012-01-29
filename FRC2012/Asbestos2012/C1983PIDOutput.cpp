#include "C1983PIDOutput.h"

C1983PIDOutput::C1983PIDOutput(Jaguar *jag01, Jaguar *jag02,bool reversed)
{
	jag1 = jag01;
	jag2 = jag02;
	reverse = reversed;
}

void C1983PIDOutput::PIDWrite(float output)
{
	if(reverse)
	{
		jag1->Set(-output);
		jag2->Set(-output);
	}else{
		jag1->Set(output);
		jag2->Set(output);
	}
}
