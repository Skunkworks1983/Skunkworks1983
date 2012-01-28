/*
 * C1983Collector.cpp
 *
 *  Created on: Jan 14, 2012
 *      Author: Austin
 *
#include "C1983Collector.h"

C1983Collector::C1983Collector()
{
	//Victor used for feeding
	feedVic = new Victor (COLLECTOR_FEED_VIC);

	//First victor used for collector
	collectorVicBottom = new Victor (COLLECTOR_VIC_BOTTOM);

	//Second victor used for collecting
	collectorVicTop = new Victor (COLLECTOR_VIC_TOP);

	//The auto feed for the collector, setting it to true.
	autoFeed = true;
}

void C1983Collector::feed()
{
	//Runs the ball through the collector.

	//Checks to see if the goal slot is empty.
	if (goalSlot != kNull)
	{
		//If the goal slot is the shooter which is controlled by the driver, do this.
		if (goalSlot == kShooter)
		{
			//Set the speed of the top and bottom victor.
			collectorVicTop->Set(COLLECTORDRIVESPEED);
			collectorVicBottom->Set(COLLECTORDRIVESPEED);
		}
		
		//If the top microswitch has found that a ball has shot, then do the following:
		if (ballInSlot(goalSlot))
		{
			//Set the top slot under the shooter as empty, which will allow the autofeed to to move the balls up again.
			kTop = goalslot;
			kTop = kNull;
		}

	}

	if (goalSlot == kTop) //If the goal slot is the top, or the shooter
	{
		//Set the speed of the first collector victor to move the ball through the feeder.
		collectorVicBottom->Set(COLLECTORDRIVESPEED); //Top belt
	}

	//Set the speed of the second collector victor to move the ball through the feeder.
	collectorVicTop->Set(COLLECTORDRIVESPEED);

	//If the goal slot is filled.
	if (ballInSlot(goalSlot))
	{
		//Set the goal slot as full.
		goalSlot = kNull;
	}
}

//if the goal slot for the feeder is full.
if (goalSlot == kNull)
{
	//Stop the victors in the collector.
	collectorVicBottom->Set(0.0); //If there isn't a goal stop the victors
	collectorVicTop->Set(0.0);

	if (autoFeed)//Auto feed the next possible ball

	{
		//Setting the area after the shot equal to variable i.
		for (int i = kTop + 1; i <= kBottom; i++)
		{
			//Checks if the ball has been shot and there is not a ball in the area underneath it.
			if (ballInSlot(i) && !ballInSlot(i - 1))
			{
				//Feed the ball, and balls below up a level
				goalSlot = i - 1;
			}
		}
	}

}
}

int C1983Collector::getBallCount()
{
//If the goalslot is full.
if (goalSlot == kNull)
{

	//Set the total ball count to zero.
	cacheBallCount = 0;

	//When i is less then the slots availiable in the feeder add one ball to the total ball count.
	for (int i = 0; i < COLLECTOR_SLOT_COUNT; i++)
	{
		if (ballInSlot(i))
		{
			cacheBallCount++;
		}
	}
}
return cacheBallCount;
}

bool C1983Collector::ballInSlot(int slot)
{
	return false;
}

//Sees if the collector is feeding balls.
bool C1983Collector::isFeeding()
{
	//Makes sure the collector victors are equal to zero to see if the collector is feeding.
	return collectorVicBottom->Get() != 0 || collectorVicTop->Get() != 0;
}
*/
