#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPIlib.h"
#include "1983Defines2012.h"

class C1983Collector
{

private:		
		Jaguar *collectorJag1; //Jaguars for the shooting wheels
		Jaguar *collectorJag2;
		Jaguar *collectorJag3;
		Jaguar *collectorJag4;


public:
		C1983Collector();
		
		// Reads if a ball has entered the conveyor.
		//bool ballEnter();

		//Feeds the ball through the collector.
		void doFeed();

		// Checks the amount of balls that has entered the collector.
		int getBallCount();
		
		//Checks if a bell is in a slot.  Top-Bottom, starting at 0
		bool ballInSlot(int slot);
};
#endif
