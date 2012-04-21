/*
 * C1983GlyphCamera.h
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */
#if TRACKING_CAMERA
#ifndef __C1983GLYPHCAMERA_H
#define __C1983GLYPHCAMERA_H

#include "server1180.h"
#include "1983Defines2012.h"
#include "Utility.h"

class C1983GlyphCamera
{
private:
	enum Camera
	{
		kShooter, kCollector
	};
	double currentBasketYaw;
	double currentBasketDepth;
	double basketDataUpdate;

	double currentBallYaw;
	double currentBallDepth;
	double ballDataUpdate;

	Camera currentCamera;

	Server1180 * server;
public:
	C1983GlyphCamera();
	void processPacket(char * data);
	static void callProcessPacket(void * cam, char * data);
	double getCurrentBasketYaw();
	double getCurrentBasketDepth();

	double getCurrentBallYaw();
	double getCurrentBallDepth();

	double getBallDataAge();
	double getBasketDataAge();

	void sendCameraChange(Camera camera);
};
#endif
#endif

