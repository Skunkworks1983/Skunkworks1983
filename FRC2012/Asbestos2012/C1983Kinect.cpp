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
	kinect = Kinect::GetInstance();
	hipPositionCache = -1;
	shiftedHigh = true;
	kinectMode = false;
	tip = false;
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
	if (rightArm->GetRawButton(0))
		shiftedHigh = true;
	if (rightArm->GetRawButton(1))
		shiftedHigh = false;
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

bool C1983Kinect::getKinectMode()
{
	if (!kinectMode)
	{
		kinectMode = rightArm->GetRawButton(2) || rightArm->GetRawButton(3);
	}
	return kinectMode;
}

void C1983Kinect::update()
{
	Skeleton skele = kinect->GetSkeleton(1);
	Skeleton::Joint hipCenter = skele.GetHipCenter();
	if (hipCenter.trackingState == Skeleton::kTracked)
	{
		if (hipPositionCache == -1)
		{
			hipPositionCache = hipCenter.y;
			cout << "Set default depth at: " << hipCenter.y << endl;
		}
		float diff = hipCenter.y - hipPositionCache;
		if (diff < -KINECT_HIP_DIFF)
		{
			cout << "TIP UP\t";
		} else if (diff > KINECT_HIP_DIFF)
		{
			cout << "TIP DOWN\t";
		}
		cout << "Diff: " << diff << endl;
	}
	tip = false;
}

bool C1983Kinect::getTipper()
{
	return tip;
}
