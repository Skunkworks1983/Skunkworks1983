#ifndef __C1983VICTORCALIBRATION_H
#define __C1983VICTORCALIBRATION_H

#include "WPILib.h"

class C1983VictorCalibration : public SimpleRobot {
private:
	Victor * victor;
	Joystick * stick;
public:
	C1983VictorCalibration();
	void Autonomous();
	void OperatorControl();
	void Disabled();
	~C1983VictorCalibration();
};