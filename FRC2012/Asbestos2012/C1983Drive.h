#ifndef __C1983DRIVE_H
#define __C1983DRIVE_H
#include "1983Defines2012.h"
#include "WPILib.h"
#include "C1983PIDOutput.h"
#include "C1983PIDSource.h"
#include "C1983TurnPIDSource.h"
#include "C1983TurnPIDOutput.h"
#include "C1983Tipper.h"
#include <math.h>

class C1983Drive
{
private:
#if !(GOGO)
	Victor *leftVic1;
#endif
	Victor *leftVic2;
	Victor *rightVic1;
	Victor *rightVic2;
	
	//Begin Drive PIDs
	PIDController *leftPID;
	PIDController *rightPID;

	C1983PIDSource *leftPIDSource;
	C1983PIDSource *rightPIDSource;
	
	C1983PIDOutput *leftPIDOutput;
	C1983PIDOutput *rightPIDOutput;
	//End Drive PIDs
	
	//TurnPID
	C1983TurnPIDSource *turnPIDSource;
	C1983TurnPIDOutput *turnPIDOutput;
	//End TurnPID
	
	Encoder *leftEncoder;
	Encoder *rightEncoder;
	
	//Compressor Relay
	Relay *compressor;
	DigitalInput *compressorSwitch;
	
	//Shifter Relay
	Solenoid *shiftHigh;
	Solenoid *shiftLow;
	
	//Poker
	Solenoid *pokerUp;
	Solenoid *pokerDown;
	
	//Tipper
	
	//Light Relay
	Relay *light;
	
	//The light sensors
	DigitalInput *lightSensorFront;
	DigitalInput *lightSensorBack;
	DigitalInput *lightSensorBridge;
	
	Gyro *gyro;
	
public:
	C1983Tipper *tipper;
	bool shiftedHigh;
	bool pokedUp;
	bool lightState;
	
	C1983Drive();
	//Set the speed of the left side -1 to 1
	void setSpeedL(float speed);
	
	//Set the speed of the right side
	void setSpeedR(float speed);
	
	//Set the compressor to the proper state
	void updateCompressor();
	
	void shift(bool high);
	
	void poke(bool up);
	
	void tip(bool down);
	
	void setLight(bool on);
	
	bool getLight();
	
	bool getLightSensorBack();
	
	bool getLightSensorFront();
	
	bool getLightSensorBridge();

	bool isShiftedHigh();
	
	
	float getGyro();
	void resetGyro();
	
	double getR();
	double getL();
	
	double getRPosition();
	double getLPosition();
	
	void resetEncoders();
#if DRIVE_PID
	void iUp();
	void iDown();
	void pUp();
	void pDown();
	void dUp();
	void dDown();
	
	void resetLeftI();
	void resetRightI();
	void cleanPID();
	void enablePID();
	void disablePID();
	
	float getLError();
	
	float getI();
	float getP();
	float getD();
	float getLSetpoint();
	float getRSetpoint();
	double getLPercent();
	double getRPercent();
#endif	
	void debugPrint();
	
	//Begin Turn PID
	PIDController *turnPID;
	//End Turn PID

	~C1983Drive();
};
#endif
