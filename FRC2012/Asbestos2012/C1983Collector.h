#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPILib.h"
#include "1983Defines2012.h"

class C1983Collector {

private:
	Victor *collectorVic1; //Victors for the collection belts
	Victor *collectorVic2;
	Victor *feedVic;
	int goalSlot;
	bool autoFeed; //Automatically feed balls in the collector

	typedef enum {kTop, kMid, kBottom,kNull} SlotName;

public:
	C1983Collector();

	// Reads if a ball has entered the conveyor.
	//bool ballEnter();

	//Feeds the ball through the collector.
	void feed();

	// Checks the amount of balls that has entered the collector.
	int getBallCount();

	//Checks if a bell is in a slot.  Top-Bottom, starting at 0
	bool ballInSlot(int slot);
};
#endif
