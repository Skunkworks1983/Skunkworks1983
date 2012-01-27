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
}

float C1983Kinect::getLeft()
{
	return leftArm->GetY();
}
float C1983Kinect::getRight()
{
	return rightArm->GetY();
}