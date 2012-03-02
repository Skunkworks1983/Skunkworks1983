/*
 * C1983GlyphCamera.h
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */
#ifndef __C1983GLYPHCAMERA_H
#define __C1983GLYPHCAMERA_H

#include "server1180.h"

class C1983GlyphCamera
{
private:
	double currentYaw;
	double currentDepth;
	Server1180 * server;
public:
	C1983GlyphCamera();
	void processPacket(char * data);
	static void callProcessPacket(void * cam, char * data);
	double getCurrentYaw();
	double getCurrentDepth();
	bool hasData();
};
#endif

