#include "PewPewBot.h"
#include <iostream>

double PewPewBot::currentTimeMillis()
{
	struct timespec tt;
	clock_gettime(CLOCK_REALTIME, &tt);
	double diff = (tt.tv_sec*1000.0) + (double)(tt.tv_nsec /1000000.0);
	return diff;
}

PewPewBot::PewPewBot()
{
	drive = new C1983Drive();
	shooter = new C1983Shooter();
	collector = new C1983Collector(shooter);
	//camera = new C1983GlyphCamera();
#if KINECT
	kinect = new C1983Kinect();
#endif
	lStick = new Joystick(2);
	rStick = new Joystick(1);

	driverStation = DriverStation::GetInstance();
	driverStationLCD = DriverStationLCD::GetInstance();
	myEIO = &driverStation->GetEnhancedIO();
#if SHOOTER_PID
	shooter->setPIDAdjust(0.0);
#endif

	hasResetItem = false;
	yawAlignState = false;
	flashState = false;
	stableCount = 0;
	nextFlash = 0;
	shooter->setEnabled(false);
}

PewPewBot::~PewPewBot()
{
}

void PewPewBot::updateDriverStation()
{
	if (PewPewBot::currentTimeMillis() > nextFlash){
		flashState = !flashState;
		nextFlash = PewPewBot::currentTimeMillis() + LED_FLASH_RATE;
	}
	//Blinken Lights
	FRONT_LINE_LED(drive->getLightSensorFront() || (drive->getLightSensorBridge() && flashState));
	BACK_LINE_LED(drive->getLightSensorBack() || (drive->getLightSensorBridge() && flashState));
	RPM_LOCK_LED(shooter->isReady());
	AUTO_RANGE_LED(false);
	AUTO_YAW_LED(false);

	//Drive Station LCD
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line1,
			"Shooter RPM: %lf", shooter->getRate() * SHOOTER_MAX_SPEED);
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line2,
			"Requested RPM: %f", (int)shooter->getSetpoint());
#if SHOOTER_PID
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line3,
			"Modification: %f", shooter->getPIDAdjust());
	/*
	 driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Yaw: %lf", camera->getCurrentYaw());
	 driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line5, "Depth: %lf", camera->getCurrentDepth());
	 */
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line4, "P: %f",
			shooter->getP());
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line5, "I: %f",
			shooter->getI());
	driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line6, "D: %f",
			shooter->getD());
#endif
	//driverStationLCD->PrintfLine(DriverStationLCD::kUser_Line4, "Bridge: %s", (drive->getLightSensorBridge()?"True":"False"));
	driverStationLCD->UpdateLCD();
}

void PewPewBot::updateShooter()
{
	if (!SHOOTER_MANUAL_MODE && SHOOTER_PID)
	{
		shooter->setEnabled(true);
		//Updates the average. Maybe some other stuff later.
		shooter->update();
		//Shot select
		if (SHORT_SHOT_SWITCH)
		{
			shooter->setShot(C1983Shooter::kFreethrow);
		} else
		{
			shooter->setShot(C1983Shooter::kKeytop);
		}
		//PID Adjust
		if (PID_SLIDER> .2095)
		{
			shooter->setPIDAdjust(0.0762 * log(PID_SLIDER) + 0.0407);
		} else
		{
			shooter->setPIDAdjust(-0.15);
			shooter->setEnabled(ARM_BUTTON);
		}
	} else
	{
		shooter->setEnabled(false);
		shooter->setJankyPower((0.0762 * log(PID_SLIDER) + 0.1907) /0.30);
	}
}

void PewPewBot::OperatorControl()
{
	int count = 0;//DEBUG (C1983)
	bool shifterToggle = false;
	shooter->setEnabled(false);
	GetWatchdog().SetEnabled(true);
	GetWatchdog().SetExpiration(0.1);
#if DRIVE_PID
	drive->enablePID();
#endif
	//drive->turnPID->Enable();
	//Cleaning
	collector->clean();

	while (IsOperatorControl() && !IsDisabled())
	{
		if (count++ > 5)
		{
			count = 0;
			updateDriverStation();
#if DRIVE_PID
			//drive->debugPrint();
			//cout << endl;
#endif
			/*cout<<"LSpeed: "<<drive->getL()<<" RSpeed: "<<drive->getR()+++++++++++++++++++++----------
			 <<" Speed/Ideal Max: "<<drive->getL()/MAXSPEEDHIGH
			 << "Shooter Power: "<<-(rStick->GetThrottle() + 1)/2<<endl;*/
			//cout<<"Shooter Rate:" << shooter->getRate()<<endl;
			//shooter->debugPrint();
			//collector->debugPrint();
		}

		//Set the compressor
		drive->updateCompressor();

		/*if (yawAlignState && !AUTO_TARGET_BUTTON)
		 {
		 cout << "Force ending turn cycle" << endl;
		 yawAlignState = false;
		 drive->turnPID->Disable();
		 drive->enablePID();
		 hasResetItem = false;
		 }
		 if (AUTO_TARGET_BUTTON)
		 {
		 camYawAlign();
		 } else */if (KEY_ALIGN_BUTTON)
		{
			lineDepthAlign();
		} else
		{
			//Set the drive base to the stick speeds (Joysticks are backwards yo!)
			if (fabs(rStick->GetY()) >= DEADBAND)
			{
				drive->setSpeedR(-rStick->GetY());
			} else
			{
				drive->setSpeedR(0.0);
			}
			if (fabs(lStick->GetY()) >= DEADBAND)
			{
				drive->setSpeedL(-lStick->GetY());
			} else
			{
				drive->setSpeedL(0.0);
			}
		}

		//check for shifting
		if (!shifterToggle && (shifterToggle = SHIFT_BUTTON))
		{
			drive->shift(!(drive->shiftedHigh));
		} else
		{
			shifterToggle = SHIFT_BUTTON;
		}

		//COLLECTOR
		if (COLLECT_BUTTON)
		{
			collector->setAutomatic(true);
			collector->requestCollect();
		} else if (REVERSE_SWITCH)
		{
			collector->setAutomatic(false);
			collector->requestReverse();
		} else if (FORWARD_SWITCH)
		{
			collector->setAutomatic(false);
			collector->requestCollect();
		} else
		{
			collector->requestStop();
		}
		collector->update();
		//END COLLECTOR

		//SHOOTER	
		//Updates the average. Maybe some other stuff later.
#if SHOOTER_PID
		shooter->update();
		if (SHORT_SHOT_SWITCH)
		{
			shooter->setShot(C1983Shooter::kFreethrow);
		} else
		{
			shooter->setShot(C1983Shooter::kKeytop);
		}
		shooter->setEnabled(ARM_BUTTON);
#else
		shooter->update();
		shooter->setJankyPower(1.0);
#endif
		//Check for shot
		if (SHOOT_BUTTON)
		{
			collector->requestShot();
		}
		//END SHOOTER
		//TIPPER
		drive->tip(TIPPER_SWITCH);
		//END TIPPER
#if SHOOTER_PID
		if (PID_SLIDER> .2095)
		{
			shooter->setPIDAdjust(0.0762 * log(PID_SLIDER) + 0.0407);
		} else
		{
			shooter->setPIDAdjust(-0.15);
		}

#endif

		//Check for PID modification DEBUG
#if DRIVE_PID
#if SHOOTER_PID
		/*if (lStick->GetRawButton(4) && count/5 == (float)count/5)
		 {
		 shooter->pDown();
		 }
		 if (lStick->GetRawButton(5) && count/5 == (float)count/5)
		 {
		 shooter->pUp();
		 }

		 if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		 {
		 shooter->iDown();
		 }

		 if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		 {
		 shooter->iUp();
		 }

		 if (rStick->GetRawButton(2) && count/5 == (float)count/5)
		 {
		 shooter->dDown();
		 }

		 if (rStick->GetRawButton(3) && count/5 == (float)count/5)
		 {
		 shooter->dUp();
		 }*/
#endif

		if (lStick->GetRawButton(5))
		{
			drive->resetLeftI();
			drive->resetRightI();
		}
#endif
		GetWatchdog().Feed();
		myfile<<count<<","<<drive->getL()<<"\n";
		Wait(0.02);
	}
}

void PewPewBot::Disabled()
{
	//drive->resetEncoders();
#if SHOOTER_PID
	shooter->setEnabled(false);
	shooter->cleanPID();
	collector->clean();
	GetWatchdog().SetEnabled(false);
#endif
	//close file
#if DRIVE_PID
	drive->cleanPID();
#endif
	//Cleaning
	collector->clean();
	while (IsDisabled())
	{
		Wait(0.02);
	}
}

void PewPewBot::cleanPIDs()
{
	drive->cleanPID();
#if SHOOTER_PID
	shooter->cleanPID();
#endif
}

START_ROBOT_CLASS(PewPewBot)
;
