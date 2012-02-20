#ifndef __Bot_H
#define __Bot_H

#include "WPILib.h"
#include "vxWorks.h"
#include <iostream>
#include "C1983Encoder.h"
using namespace std;

class Bot : public SimpleRobot
{
public:
	Bot();
	
	void Autonomous();
	void OperatorControl();
	void Disabled();
	
	~Bot();
private:
	Victor *shooterVic1;
	Victor *shooterVic2;
	C1983Encoder *encoder;
};
#endif
