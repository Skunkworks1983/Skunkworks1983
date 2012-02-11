#if 0
#ifndef __C1983SHOOTER_H
#define __C1983SHOOTER_H
#include "WPIlib.h"
#include "1983Defines2012.h"

class C1983Shooter {
private:
	Victor *shooterVic; //Victor for the shooting wheel
	Victor *hoodVic; //Victor for the hood
	AnalogChannel * hoodPot; //Pot that reads the current hood angle
	Encoder *wheelEncoder; //Encoder that reads the shooting wheel

	//PID Control
	PIDController *hoodPID;
	PIDController *wheelPID;
	//End PID Control

	float targetVelocity;
	float targetAngle;
public:
	enum ShotType {
		kLayup, kFreeThrow, kOther
	};
	C1983Shooter();
	//shooting function
	void shoot();

	//setting the velocity needed
	void setVelocity(float velocity);

	//Set the angle that is needed to shoot.
	void setAngle(float angle);
	
	//Checks to see if ball is ready to be shot
	bool isReady();

	//Gets the cordinates of the hoop
	//void getCoordinates();
	//void getCorrections();

	//Get the current wheel speed
	double getVelocity();

	//Get the angle needed to shoot at the hoop.
	float getAngle();
	
	//sets the spead and angle to the presets!
	void aim(int preset);
};
#endif
#endif