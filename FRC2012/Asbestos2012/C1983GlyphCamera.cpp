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
	currentDepth = -1;
	currentYaw = -1;
	server = new Server1180(C1983GlyphCamera::callProcessPacket, this, true);
}
double C1983GlyphCamera::getCurrentDepth()
{
	return currentDepth;
}
double C1983GlyphCamera::getCurrentYaw()
{
	return currentYaw;
}

void C1983GlyphCamera::callProcessPacket(void *obj, char * data)
{
	((C1983GlyphCamera*)obj)->processPacket(data);
}
void C1983GlyphCamera::processPacket(char * data)
{
	sscanf(data, "%lf,%lf", &currentDepth, &currentYaw);
}

bool C1983GlyphCamera::hasData(){
	return currentDepth != -1;  //TODO  Check the current time against the last update time
}
