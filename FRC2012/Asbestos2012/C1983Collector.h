#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPILib.h"
#include "1983Defines2012.h"
#include "C1983Shooter.h"

//IR Sensor Macros
#define LOWSLOT (lowSlot->Get() == 1)
#define MIDSLOT (midSlot->Get() == 1)
#define TOPSLOT (topSlot->Get() == 1)

class C1983Collector {

private:
	Victor *collectorVicLow; //Victors for the collection belts
	Victor *collectorVicTop;
	Victor *collectorVicPickup;

	DigitalInput * lowSlot;
	DigitalInput * midSlot;
	DigitalInput * topSlot;
	C1983Shooter * shooter;

	bool lowLastState;
	bool collectorTransition;
	bool shooting;
	bool collecting;
	int collectorCount;
	int shooterCount;
	bool manual;
	bool forward;
	bool beganShotCheck;
public:
	C1983Collector(C1983Shooter *sh);

	// Reads if a ball has entered the conveyor.
	//bool ballEnter();

	void update();
	
	//Sets the collector right
	void requestCollect();
	
	//Feeds to the shooter
	void requestShot();
	
	void jankyGo();
	
	void jankyStop();
	
	bool getSense(int height);
};
#endif
