#ifndef __C1983DRIVE_H
#define __C1983DRIVE_H
#include "1983Defines2012.h"
#include "WPILib.h"
#include "C1983PIDOutput.h"

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
	
	PIDSource *leftPIDSource;
	PIDSource *rightPIDSource;
	
	PIDOutput *leftPIDOutput;
	PIDOutput *rightPIDOutput;
#endif
	//Encoder *fakeEncoder1;
	Encoder *leftEncoder;
	//Encoder *fakeEncoder2;
	Encoder *rightEncoder;

	//Compressor Relay
	Relay *compressor;
	DigitalInput *compressorSwitch;
	
	//Shifter Relay
	Relay *shifter;
	
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
	
	int getL();
	
	int getR();
	
	/*
	//Get the speed of the left side
	float getSpeedL();
	
	//Get the speed of the right side
	float getSpeedR();
	
	
	//Reset the PID's I term
	float resetI();
	
	//Move a certain distance
	void move(float distance);
	*/
	~C1983Drive();
};
#endif
