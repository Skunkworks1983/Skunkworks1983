#ifndef __C1983SHOOTER_H
#define __C1983SHOOTER_H
#include "WPIlib.h"
#include "1983Defines2012.h"

class C1983Shooter
{

private:
	Victor *shooterVic; //Victor for the shooting wheel
	Victor *hoodVic; //Victor for the hood

	//Encoder *encoder1;
	//Encoder *encoder2;

public:
	C1983Shooter();
	//Does the shooter keep itself up to speed
	bool isWheelRunning;
	
	//shooting function
	void shoot();

	//setting the velocity needed
	void setVelocity(float velocity);

	//Checks to see if ball is ready to be shot
	bool isReady();

	//Gets the cordinates of the hoop
	//void getCoordinates();
	//void getCorrections();

	//Get the current wheel speed
	float getVelocity();

	//Get the angle needed to shoot at the hoop.
	float getAngle();

	//Set the angle that is needed to shoot.
	void angle(float angle);
	
	//sets the spead and angle to the presets!
	void aim(int Preset=0);
	
	//shooter class variable for speed
	float targetSpeed;
	//shooter class global variable for angle
	float targetAngle;
};
#endif
