#include "C1983Collector.h"

C1983Collector::C1983Collector(C1983Shooter *sh) {
	shooter = sh;
	//Victor used for feeding
	collectorVicPickup = new Victor (COLLECTOR_VIC_PICKUP);

	//First victor used for collector
	collectorVicLow = new Victor (COLLECTOR_VIC_LOW);

	//Second victor used for collecting
	collectorVicTop = new Victor (COLLECTOR_VIC_TOP);

	//Sensors
	lowSlot = new DigitalInput(COLLECTOR_IR_LOW_CHANNEL);
	midSlot = new DigitalInput(COLLECTOR_IR_MID_CHANNEL);
	topSlot = new DigitalInput(COLLECTOR_IR_TOP_CHANNEL);
	
	lowLastState = false;
	collectorTransition = false;
	collectorCount = 0;
	
	manual = false;
	forward = true;
	//The auto feed for the collector, setting it to true.
}

void C1983Collector::update()
{	
	if(manual && forward) //Manual forward
	{
		collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(COLLECTOR_BELT_SPEED);
	}else if(manual){ //Manual reverse
		collectorVicPickup->Set(-COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(-COLLECTOR_BELT_SPEED);
	}else if(collecting && !shooting){ 
		if(collectorCount > COLLECTOR_TIMEOUT)
		{
			collectorVicPickup->Set(0.0);
			collectorVicLow->Set(0.0);
			collectorVicTop->Set(0.0);
			collecting = false;
			collectorCount = 0;
		}else if(!TOPSLOT){
			collectorVicTop->Set(COLLECTOR_BELT_SPEED);
			collectorVicLow->Set(COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		}else if(!MIDSLOT){
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);			
		}else if(!LOWSLOT){
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(0.0);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		}else{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(0.0);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		}
		collectorCount++;
	}else if(shooting && shooter->isReady()){
		collectorVicTop->Set(COLLECTOR_BELT_SPEED);
		if(SHOT_AWAY_SWITCH)
		{
			collectorVicTop->Set(0.0);
			shooting = false;
			collecting = true;
		}
	}

}

void C1983Collector::requestShot(){
	shooting = true;
}

void C1983Collector::requestCollect(){
	collecting = true;
}
