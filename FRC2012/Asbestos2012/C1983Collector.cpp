#include "C1983Collector.h"

C1983Collector::C1983Collector(C1983Shooter *sh)
{
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
	shooterCount = 0;

	shooting = false;
	collecting = false;
	automatic = false; //TODO if we have sensors, make this true
	runInReverse = false;
	beganShotCheck = false;

}

void C1983Collector::update()
{
	if (runInReverse)//Manual reverse
	{
		collectorVicPickup->Set(-COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(-COLLECTOR_BELT_SPEED);
	} else if (!automatic && collecting) //Manual forward
	{
		collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(COLLECTOR_BELT_SPEED);
	} else if (automatic && collecting && !shooting)
	{
		if (collectorCount> COLLECTOR_TIMEOUT)
		{
			collectorVicPickup->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicTop->Set(0.0);
			collecting = false;
			collectorCount = 0;
		} else if (!TOPSLOT)
		{
			collectorVicTop->Set(COLLECTOR_BELT_SPEED);
			collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		} else if (!MIDSLOT)
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		} else if (!LOWSLOT)
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		} else
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicPickup->Set(0.0);
		}
		collectorCount++;
	} else if (shooting && shooter->isReady())
	{
		shooterCount++;
		collectorVicTop->Set(COLLECTOR_FEED_SPEED);
		if (shooterCount > SHOOTER_TIMEOUT)
		{
			collectorVicTop->Set(0.0);
			shooting = false;
			shooterCount = 0;
			collecting = automatic;
			//If we want to autocollect after shooting we need to be in auto mode
		}
	}
}

void C1983Collector::requestShot()
{
	shooting = true;
}

void C1983Collector::requestCollect()
{
	collecting = true;
}

void C1983Collector::requestStop()
{
	runInReverse = false;
	collecting = collecting && automatic; //Only works in manual mode
}

void C1983Collector::requestReverse()
{
	runInReverse = true;
}

bool C1983Collector::isShooting()
{
	return shooting;
}

bool C1983Collector::isCollecting()
{
	return collecting;
}

bool C1983Collector::getSense(int height)
{
	switch (height)
	{
	case 0:
		return LOWSLOT;
		break;
	case 1:
		return MIDSLOT;
		break;
	case 2:
		return TOPSLOT;
		break;
	}
	return 0;
}

void C1983Collector::jankyGo()
{
	collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
	collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
	collectorVicTop->Set(COLLECTOR_BELT_SPEED);
}

void C1983Collector::jankyStop()
{
	collectorVicPickup->Set(0.0);
	collectorVicLow->Set(0.0);
	collectorVicTop->Set(0.0);
}
