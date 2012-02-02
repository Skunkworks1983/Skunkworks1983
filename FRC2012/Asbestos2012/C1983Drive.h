#ifndef __C1983DRIVE_H
#define __C1983DRIVE_H
#include "1983Defines2012.h"
#include "WPILib.h"
#include "C1983PIDOutput.h"
#include "C1983PIDSource.h"

class C1983Drive
{
private:
	Jaguar *leftJag1;
	Jaguar *leftJag2;
	Jaguar *rightJag1;
	Jaguar *rightJag2;
#if USE_PID
	PIDController *leftPID;
	PIDController *rightPID;

	C1983PIDSource *leftPIDSource;
	C1983PIDSource *rightPIDSource;
	
	PIDOutput *leftPIDOutput;
	PIDOutput *rightPIDOutput;
#endif
	Encoder *leftEncoder;
	Encoder *rightEncoder;
	
	//Compressor Relay
	Relay *compressor;
	DigitalInput *compressorSwitch;
	
	//Shifter Relay
	Relay *shifter;
	
	//Light Relay
	Relay *light;
	
	//The light sensors
	DigitalInput *lightSensorFront;
	DigitalInput *lightSensorBack;
	
	bool shiftedHigh;
	
public:
	C1983Drive();
	//Set the speed of the left side -1 to 1
	void setSpeedL(float speed);
	
	//Set the speed of the right side
	void setSpeedR(float speed);
	
	//Set the compressor to the proper state
	void updateCompressor();
	
	void shift(bool high);
	
	void setLight(bool on);
	
	bool getLightSensorBack();
	
	bool getLightSensorFront();

	double getR();
	double getL();
	
#if USE_PID
	void iUp();
	void iDown();
	void pUp();
	void pDown();
	
	void resetLeftI();
	void resetRightI();
	
	float getLError();
	
	
	float getI();
	float getP();
	float getLSetpoint();
	float getRSetpoint();
	double getLPercent();
	double getRPercent();
	
#endif
	
	~C1983Drive();
};
#endif
