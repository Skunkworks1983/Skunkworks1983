/*
 * C1983Kinect.cpp
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */

#include "C1983Kinect.h"

C1983Kinect::C1983Kinect()
{
	leftArm = new KinectStick(1);
	rightArm = new KinectStick(2);
	shiftedHigh = true;
}

float C1983Kinect::getLeft()
{
	return leftArm->GetY();
}
float C1983Kinect::getRight()
{
	return rightArm->GetY();
}

bool C1983Kinect::getShiftButton()
{
	if(rightArm->GetRawButton(0)) shiftedHigh = true;
	if(rightArm->GetRawButton(1)) shiftedHigh = false;
	return shiftedHigh;
}

bool C1983Kinect::getShootButton()
{
	return rightArm->GetRawButton(1);
}

bool C1983Kinect::getShooterOnButton()
{
	return rightArm->GetRawButton(0);
}
