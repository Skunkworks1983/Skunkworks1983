#include "C1983Collector.h"

C1983Collector::C1983Collector(C1983Shooter * sh, C1983Tipper * tip)
{
	shooter = sh;
	tipper = tip;
	
	//Victor used for feeding
	collectorVicPickup = new Victor (COLLECTOR_VIC_PICKUP);

	//Ye old tipper victor
#ifdef COLLECTOR_VIC_TIPPER
	collectorVicTipper = new Victor(COLLECTOR_VIC_TIPPER);
#endif
	
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
	automatic = true;  //We haz light sensors
	runInReverse = false;

}

void C1983Collector::update()
{
	if (!automatic && runInReverse && !shooting)//Manual reverse
	{
		collectorVicPickup->Set(-COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(-COLLECTOR_BELT_SPEED);
#ifdef COLLECTOR_VIC_TIPPER
			if(tipper->getState())
			{
				collectorVicTipper->Set(-COLLECTOR_TIPPER_SPEED);
			}else{
				collectorVicTipper->Set(0.0);
			}
#endif
	} else if (!automatic && collecting && !shooting) //Manual forward
	{
		collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
		collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
		collectorVicTop->Set(COLLECTOR_BELT_SPEED);
#ifdef COLLECTOR_VIC_TIPPER
			if(tipper->getState())
			{
				collectorVicTipper->Set(COLLECTOR_TIPPER_SPEED);
			}else{
				collectorVicTipper->Set(0.0);
			}
#endif
	}else if (shooting)	{
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
	} else if (automatic && collecting && !shooting)
	{
		if (collectorCount > COLLECTOR_TIMEOUT)
		{
			collectorVicPickup->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicTop->Set(0.0);
#ifdef COLLECTOR_VIC_TIPPER
			collectorVicTipper->Set(0.0);
#endif
			collecting = false;
			collectorCount = 0;
		} else if (!TOPSLOT)
		{
			collectorVicTop->Set(COLLECTOR_BELT_SPEED);
			collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
#ifdef COLLECTOR_VIC_TIPPER
			if(tipper->getState())
			{
				collectorVicTipper->Set(COLLECTOR_TIPPER_SPEED);
			}else{
				collectorVicTipper->Set(0.0);
			}
#endif
		} else if (!MIDSLOT)
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
#ifdef COLLECTOR_VIC_TIPPER
			if(tipper->getState())
			{
				collectorVicTipper->Set(COLLECTOR_TIPPER_SPEED);
			}else{
				collectorVicTipper->Set(0.0);
			}
#endif
		} else if (!LOWSLOT)
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
#ifdef COLLECTOR_VIC_TIPPER
			if(tipper->getState())
			{
				collectorVicTipper->Set(COLLECTOR_TIPPER_SPEED);
			}else{
				collectorVicTipper->Set(0.0);
			}
#endif
		} else
		{
			collectorVicTop->Set(0.0);
			collectorVicLow->Set(-0.0);
			collectorVicPickup->Set(0.0);
#ifdef COLLECTOR_VIC_TIPPER
			collectorVicTipper->Set(0.0);
#endif
		}
		collectorCount++;
	} else
	{
		collectorVicTop->Set(0.0);
		collectorVicLow->Set(-0.0);
		collectorVicPickup->Set(0.0);
#ifdef COLLECTOR_VIC_TIPPER
		collectorVicTipper->Set(0.0);
#endif
	}
}

void C1983Collector::requestShot()
{
	shooting = shooting || shooter->isStableReady();
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

void C1983Collector::setAutomatic(bool bleh)
{
	automatic = bleh;
}

void C1983Collector::jankyGo()
{
#ifdef COLLECTOR_VIC_TIPPER
	if(tipper->getState())
	{
		collectorVicTipper->Set(COLLECTOR_TIPPER_SPEED);
	}else{
		collectorVicTipper->Set(0.0);
	}
#endif
	collectorVicPickup->Set(COLLECTOR_PICKUP_SPEED);
	collectorVicLow->Set(-COLLECTOR_BELT_SPEED);
	collectorVicTop->Set(COLLECTOR_BELT_SPEED);
}

void C1983Collector::jankyReverse()
{
#ifdef COLLECTOR_VIC_TIPPER
	if(tipper->getState())
	{
		collectorVicTipper->Set(-COLLECTOR_TIPPER_SPEED);
	}else{
		collectorVicTipper->Set(0.0);
	}
#endif
	collectorVicPickup->Set(-COLLECTOR_PICKUP_SPEED);
	collectorVicLow->Set(COLLECTOR_BELT_SPEED);
	collectorVicTop->Set(-COLLECTOR_BELT_SPEED);
}

void C1983Collector::jankyStop()
{
	collectorVicTipper->Set(0.0);
	collectorVicPickup->Set(0.0);
	collectorVicLow->Set(0.0);
	collectorVicTop->Set(0.0);
}

void C1983Collector::clean()
{
	shooting = false;
	collecting = false;
	automatic = true;
	runInReverse = false;
	collectorCount = 0;
	shooterCount = 0;
}

void C1983Collector::debugPrint()
{
	//if (automatic){
	cout << "Collector Sensors: " << getSense(0) << " " << getSense(1) << " "
			<< getSense(2) << "\t";
	//}
	//cout << "Automatic: " << automatic << "\tCollecting: " << collecting << "\tShooting: " << shooting << "\tReverse: " << runInReverse;
}
