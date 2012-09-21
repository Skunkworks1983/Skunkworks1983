/*
 * C1983GlyphCamera.cpp
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */
#include "C1983GlyphCamera.h"
#if TRACKING_CAMERA

C1983GlyphCamera::C1983GlyphCamera()
{
	currentBasketDepth = -1;
	currentBasketYaw = -1;
	basketDataUpdate = -1;

	currentBallYaw = -1;
	currentBallDepth = -1;
	ballDataUpdate = -1;

	currentCamera = kShooter;

	server = new Server1180(C1983GlyphCamera::callProcessPacket, this, false);
}

void C1983GlyphCamera::callProcessPacket(void *obj, char * data)
{
	((C1983GlyphCamera*)obj)->processPacket(data);
}
void C1983GlyphCamera::processPacket(char * data)
{
	double d1, d2;
	unsigned int key;
	sscanf(data, "%u,%lf,%lf", &key, &d1, &d2);
	if (key == 0)
	{
		currentBasketDepth = d1;
		currentBasketYaw = d2;
		basketDataUpdate = System::currentTimeMillis();
	} else if (key == 1)
	{
		currentBallDepth = d1;
		currentBallYaw = d2;
		ballDataUpdate = System::currentTimeMillis();
	} else
	{
		//cout << "Bad packet key: " << key << endl;
	}
}

double C1983GlyphCamera::getCurrentBasketDepth()
{
	return currentBasketDepth;
}
double C1983GlyphCamera::getCurrentBasketYaw()
{
	return currentBasketYaw;
}

double C1983GlyphCamera::getBasketDataAge()
{
	return System::currentTimeMillis() - basketDataUpdate;
}

double C1983GlyphCamera::getCurrentBallDepth()
{
	return currentBasketDepth;
}
double C1983GlyphCamera::getCurrentBallYaw()
{
	return currentBasketYaw;
}

double C1983GlyphCamera::getBallDataAge()
{
	return System::currentTimeMillis() - ballDataUpdate;
}

void C1983GlyphCamera::sendCameraChange(Camera cam)
{
	if (server->Connected())
	{
		char * data = new char[2];
		data[0] = cam;
		data[1] = 0;
		server->SendData(data);
	}
}
#endif
