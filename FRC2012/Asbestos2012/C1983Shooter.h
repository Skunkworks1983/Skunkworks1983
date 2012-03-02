#ifndef __C1983Shooter_H
#define __C1983Shooter_H
#include "WPILib.h"
#include "math.h"
#include "1983Defines2012.h"
#include "C1983PIDOutput.h"
#include "C1983Encoder.h"
#include "C1983ShooterPIDSource.h"
#include <fstream>
#if SHOOTER
class C1983Shooter
{
private:
	Victor *shooterVic1;
	Victor *shooterVic2;
#if SHOOTER_PID
	C1983PIDOutput *shooterPIDOutput;
	C1983ShooterPIDSource *shooterPIDSource;
	PIDController *shooterPID;
#endif
	//C1983Encoder *shooterEncoder;
	Encoder *shooterEncoder;
	ofstream data;
	
	float power;
	bool manual;
	bool isEnabled;
	
public:
	C1983Shooter();
	bool isReady();
	double getRate();
	bool getEnabled();
#if SHOOTER_PID
	void setPower(float powerRPM);
	void cleanPID();
#endif
	void setShot(short shotNum);
	void setJankyPower(float power);
	void jankyStop();
	void debugPrint();
	void setEnabled(bool enabled);
#if SHOOTER_PID
	void dUp();
	void dDown();
	void pUp();
	void pDown();
	void iUp();
	void iDown();
	
	float getError();
	float getI();
	float getP();
	float getD();
	float getSetpoint();
	double getPercent();
#endif
	
	void update();
	enum {kKeytop, kFreethrow, kOther} shot;
};
#endif
#endif
