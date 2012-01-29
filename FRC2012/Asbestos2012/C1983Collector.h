#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPILib.h"
#include "1983Defines2012.h"

//IR Sensor Macros
#define LOWSLOT (lowSlot->Get() == 1)
#define MIDSLOT (midSlot->Get() == 1)
#define TOPSLOT (topSlot->Get() == 1)

class C1983Collector {

private:
	Victor *collectorVicBottom; //Victors for the collection belts
	Victor *collectorVicTop;
	Victor *feedVic;
	bool autoFeed; //Automatically feed balls in the collector
	bool lowToMid, midToTop;

	DigitalInput * lowSlot;
	DigitalInput * midSlot;
	DigitalInput * topSlot;

public:
	C1983Collector();

	// Reads if a ball has entered the conveyor.
	//bool ballEnter();

	//Sets the collector right
	void update();

	// Checks the amount of balls that has entered the collector.
	char getBallCount();
	
	//Checks to see if the collector is feeding
	bool isFeeding();
	
	//Feeds to the shooter
	void shoot();
};
#endif
