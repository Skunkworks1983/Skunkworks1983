/*
 * C1983GlyphCamera.cpp
 *
 *  Created on: Jan 25, 2012
 *      Author: Westin
 */

#include "C1983GlyphCamera.h"
#include "1983Defines2012.h"
C1983GlyphCamera::C1983GlyphCamera()
{
	camera = &AxisCamera::GetInstance(GLYPH_CAMERA_IP);
	image = new BinaryImage();
	camBuffer = new RGBImage("C:\\Users\\Westin\\some.bmp");
}
void C1983GlyphCamera::doTracking()
{
	if (camera->IsFreshImage())
	{
		if (camera->GetImage(camBuffer)==1) //Fetch
		{
			Range range1 =
			{ GLYPH_THRESH_R_L, GLYPH_THRESH_R_H };
			Range range2 =
			{ GLYPH_THRESH_G_L, GLYPH_THRESH_G_H };
			Range range3 =
			{ GLYPH_THRESH_B_L, GLYPH_THRESH_B_H };
			int success = imaqColorThreshold(image->GetImaqImage(),
					camBuffer->GetImaqImage(), 1, IMAQ_RGB, &range1, &range2,
					&range3);
			if (success==0)
				return;
			LineDescriptor * lineDescriptor = new LineDescriptor();
			lineDescriptor->minLength =10;
			lineDescriptor->maxLength = 250;
			CurveOptions * curveOptions = new CurveOptions();
			ShapeDetectionOptions * shapeDetect = new ShapeDetectionOptions();
			int * numLines;
			ROI* roi = imaqCreateROI();
			imaqAddRectContour(roi, IMAQ_NO_RECT);
			LineMatch * lines = imaqDetectLines(image->GetImaqImage(),
					lineDescriptor, curveOptions, shapeDetect, roi, numLines);
			int i;
			for (i = 0; i<*numLines; i++)
			{
				printf("Start: %f,%f End: %f,%f\n", lines[i].startPoint.x,
						lines[i].startPoint.y, lines[i].endPoint.x,
						lines[i].endPoint.y);
			}
		}
	}
}
