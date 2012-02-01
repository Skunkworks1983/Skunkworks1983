#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPILib.h"
#include "1983Defines2012.h"

//IR Sensor Macros
#define LOWSLOT (lowSlot->Get() == 1)
#define TOPSLOT (topSlot->Get() == 1)

class C1983Collector {

private:
	Victor *collectorVicBottom; //Victors for the collection belts
	Victor *collectorVicTop;
	Victor *collectorVicPickup;
	bool autoFeed; //Automatically feed balls in the collector

	DigitalInput * lowSlot;
	DigitalInput * topSlot;

public:
	C1983Collector();

	// Reads if a ball has entered the conveyor.
	//bool ballEnter();

	//Sets the collector right
	void collect();
	
	//Feeds to the shooter
	void shoot();
};
#endif
