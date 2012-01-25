/*
 * C1983Collector.cpp
 *
 *  Created on: Jan 14, 2012
 *      Author: Austin
 */

#include "C1983Collector.h"

collectorJag1 = new Jaguar (COLLECTORJAG1PORT);
collectorJag2 = new Jaguar (COLLECTORJAG2PORT);
collectorJag3 = new Jaguar (COLLECTORJAG3PORT);
collectorJag4 = new Jaguar (COLLECTORJAG4PORT);

Jag1encoder = new Encoder (JAG1COLLECTORENCODER);
fakeEncoder1 = new Encoder (22);
Jag2encoder = new Encoder (JAG2COLLECTORENCODER);
fakeEncoder2 = new Encoder (29);
Jag3encoder = new Encoder (JAG3COLLECTORENCODER);
fakeEncoder3 = new Encoder (34);
Jag4encoder = new Encoder (JAG4COLLECTORENCODER);
fakeEncoder4 = new Encoder (19);

C1983Collector::C1983Collector() 
{
	//Constructor
}

void C1983Collector::Feed()
{
	//Runs the ball through the collector.
}

void C1983Collector::isBallReadyShoot()
{
	if (sensor <= specified amount)
	{
		
	}	
}

void C1983Collector::isBallReadyStage()
{

	//Check to see if there is a ball in the collector/stage area.
}

void C1983Collector::getBallCount()
{
	//Using the infrared sensor count how many balls have entered the collector.
}

void C1983Collector::setBottom()
{
	//Not sure what set bottom means.
}

void C1983Collector::setTop()
{
	//Not sure what set top means.
}




