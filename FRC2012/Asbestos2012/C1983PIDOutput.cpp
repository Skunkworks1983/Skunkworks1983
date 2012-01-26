#include "C1983PIDOutput.h"

C1983PIDOutput::C1983PIDOutput(Jaguar *jag01, Jaguar *jag02)
{
	jag1 = jag01;
	jag2 = jag02;
}

void C1983PIDOutput::PIDWrite(float output)
{
	jag1->Set(output);
	jag2->Set(output);
}
