/*
 * C1983Collector.cpp
 *
 *  Created on: Jan 14, 2012
 *      Author: Austin
 */

#include "C1983Collector.h"

C1983Collector::C1983Collector()
{
	collectorJag1 = new Jaguar (COLLECTORJAG1PORT);
	collectorJag2 = new Jaguar (COLLECTORJAG2PORT);
	collectorJag3 = new Jaguar (COLLECTORJAG3PORT);
	collectorJag4 = new Jaguar (COLLECTORJAG4PORT);
}

void C1983Collector::doFeed()
{
	//Runs the ball through the collector.
	if (/*autoFeed*/true)//Auto feed the next possible ball
	{
		for (int i = 1; i<COLLECTOR_SLOT_COUNT; i++)
		{
			if (ballInSlot(i) && !ballInSlot(i-1))
			{
				//Feed the ball up a level
				break;
			}
		}
	}
}

int C1983Collector::getBallCount()
{
	int count = 0;
	for (int i = 0; i<COLLECTOR_SLOT_COUNT; i++)
	{
		if (ballInSlot(i))
		{
			count++;
		}
	}
	return count;
}

bool C1983Collector::ballInSlot(int slot)
{
	return false;
}

