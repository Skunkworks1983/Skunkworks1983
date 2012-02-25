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
	Relay *hoodAngler;
#if SHOOTER_PID
	C1983PIDOutput *shooterPIDOutput;
	C1983ShooterPIDSource *shooterPIDSource;
	PIDController *shooterPID;
#endif
	//C1983Encoder *shooterEncoder;
	Encoder *shooterEncoder;
	ofstream data;
	
	float goalRPM;
	bool angleHigh;
	bool active;
	float power;
	double average;
	double *values;
	double divisonNumber;
	
public:
	C1983Shooter();
	bool isReady();
	double getRate();
	void setOn(bool on);
#if SHOOTER_PID
	void setPower(float powerRPM);
#endif
	void setAngle(bool high);
	void setShot(short shotNum);
	void setJankyPower(float power);
	void jankyStop();
	void debugPrint();
	void setEnabled(bool enabled);
	bool isOn();
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
	
	void openFile();
	void closeFile();
	
	void update();
	double getAverage();
	int shooterCount;
	enum {kLayup, kFreethrow, kOther} shot;
};
#endif
#endif
