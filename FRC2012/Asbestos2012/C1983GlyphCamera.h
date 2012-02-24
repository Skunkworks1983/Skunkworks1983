/*
 * C1983GlyphCamera.h
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */
#ifndef __C1983GLYPHCAMERA_H
#define __C1983GLYPHCAMERA_H

class C1983GlyphCamera
{
private:
	double currentYaw = -1;
	double currentDepth = -1;
public:
	C1983GlyphCamera();
	void processPacket();
	double getCurrentYaw();
	double getCurrentDepth();
};
#endif

