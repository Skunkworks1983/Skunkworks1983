/*
 * C1983GlyphCamera.h
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */
#ifndef __C1983GLYPHCAMERA_H
#define __C1983GLYPHCAMERA_H

#include "WPILib.h"
#include "Vision/BinaryImage.h"
#include "Vision/RGBImage.h"

class C1983GlyphCamera
{
private:
	AxisCamera * camera;
	BinaryImage * image;
	RGBImage * camBuffer;
public:
	C1983GlyphCamera();

	//Get the location of the current set of glyphs
	void doTracking();
};
#endif

