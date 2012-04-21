/*
 * C1983Kinect.h
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 The FIRST standard gestures:
 - Left arm inclination from 0: Joystick 0 Y axis, from -128 to 127
 - Right arm inclination from 0: Joystick 1 Y axis, from -128 to 127
 
 - Head:
 - Tilted to the right: Btn0
 - Tilted to the left: Btn1        
 - Right leg:
 - Out (extended to the right): Btn2
 - Forward: Btn4
 - Backward: Btn5
 - Left leg:
 - Out (extended to the left): Btn3
 - Forward: Btn6
 - Backward: Btn7
 Button values the same for both joysticks.*/

#ifndef __C1983KINECT_H
#define __C1983KINECT_H
#include "1983Defines2012.h"
#include "WPILib.h"

class C1983Kinect
{
private:
	KinectStick *leftArm;
	KinectStick *rightArm;
	Kinect * kinect;
	float hipPositionCache;
	
	bool shiftedHigh;
	bool kinectMode;
	bool tip;
	
public:
	C1983Kinect();

	//Gets the right 'joystick' position
	float getRight();

	//Gets the left 'joystick' position
	float getLeft();
	
	bool getShiftButton();
	bool getShootButton();
	bool getShooterOnButton();
	bool getKinectMode();
	bool getTipper();
	bool hasKinect();
	
	void update();
};
#endif
