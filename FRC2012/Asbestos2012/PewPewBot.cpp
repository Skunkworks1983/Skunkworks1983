#include "PewPewBot.h"
#include <iostream>

PewPewBot::PewPewBot()
{
	drive = new C1983Drive();
	shooter = new C1983Shooter();
	collector = new C1983Collector(shooter);
#if KINECT
	kinect = new C1983Kinect();
#endif
	lStick = new Joystick(1);
	rStick = new Joystick(2);

	driverStation = DriverStation::GetInstance();
	
	hasResetItem = false;
	stableCount = 0;
}

PewPewBot::~PewPewBot()
{
}

void PewPewBot::OperatorControl()
{
	DriverStationEnhancedIO &myEIO = driverStation->GetEnhancedIO();
	int count = 0;//DEBUG (C1983)
	bool tipperToggle = false;
	bool shifterToggle = false;
	shooter->setEnabled(true);
	shooter->setOn(true);
	//shooter->setEnabled(true);
	shooter->openFile();
	myfile.open("drivedata.csv");
	myfile<<"Index,Speed\n";
	
	GetWatchdog().SetEnabled(true);
	GetWatchdog().SetExpiration(0.2);
#if DRIVE_PID
	drive->enablePID();
#endif
	//drive->turnPID->Enable();
	while (IsOperatorControl() && !IsDisabled())
	{
		count++;
		if(count % 25 == 0)
		{
			//cout<<"SpeedL: "<<drive->getL()<<" SpeedR: "<<drive->getR()<<" Throttle: "<<rStick->GetThrottle()<<" Shooter power: "<<(rStick->GetThrottle() + 1)/2<<endl;
			//cout<<"Setpoint: "<<drive->turnPID->GetSetpoint()<<" Angle: "<<drive->getGyro()<<" Error: "<<drive->turnPID->GetError()<<endl;
#if DRIVE_PID
			cout<<"SpeedL: "<<drive->getL()<<" SetpointL: "<<drive->getLSetpoint()<<" SpeedR: "<<drive->getR()<<" SetpointR: "<<drive->getRSetpoint()<<" LeftP: "<<drive->getP()<<endl;
#endif
			/*cout<<"LSpeed: "<<drive->getL()<<" RSpeed: "<<drive->getR()
			 <<" Speed/Ideal Max: "<<drive->getL()/MAXSPEEDHIGH
			 << "Shooter Power: "<<-(rStick->GetThrottle() + 1)/2<<endl;*/
			//cout<<"Shooter Rate:" << shooter->getRate()<<endl;
			shooter->debugPrint();
			cout<<endl;
		}

		//Set the compressor
		drive->updateCompressor();

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

		//check for shifting
		if (!shifterToggle && (shifterToggle = SHIFT_BUTTON))
		{
			drive->shift(!(drive->shiftedHigh));
		}else{
			shifterToggle = SHIFT_BUTTON;
		}

//COLLECTOR
		if (COLLECT_BUTTON)
		{
			collector->requestCollect();
		}else{
			collector->requestStop();
		}
		collector->update();

//SHOOTER	
		//Updates the average. Maybe some other stuff later.
#if SHOOTER_PID
		shooter->update();
		shooter->setPower(500.0/3200.0);
#else
		shooter->update();
		shooter->setJankyPower((rStick->GetThrottle() - 1)/(-2.0));
#endif
		//Check for shot
		if (SHOOT_BUTTON)
		{
			collector->requestShot();
		}
//END SHOOTER
//TIPPER
		if (!tipperToggle && (tipperToggle = TIPPER_SWITCH))
		{
			drive->tip(!(drive->tipper->getState()));
		}else{
			tipperToggle = TIPPER_SWITCH;
		}
//END TIPPER		
		//check for light
		if (LIGHT_BUTTON)
		{
			drive->setLight(true);
		} else
		{
			drive->setLight(false);
		}

		if (lStick->GetRawButton(10))
		{
			drive->resetGyro();
		}
#if SHOOTER_PID
		if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		{
			shooter->pDown();
		}
		if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		{
			shooter->pUp();
		}
#endif
		
		//Check for PID modification DEBUG
#if DRIVE_PID
		if (lStick->GetRawButton(2) && count/5 == (float)count/5)
		{
			drive->pDown();
		}
		if (lStick->GetRawButton(3) && count/5 == (float)count/5)
		{
			drive->pUp();
		}

		if (lStick->GetRawButton(5))
		{
			drive->resetLeftI();
			drive->resetRightI();
		}

		if (fabs(drive->getLSetpoint()- drive->getL()) <= .01)
		drive->resetLeftI();

		if (fabs(drive->getRSetpoint()- drive->getR()) <= .01)
		drive->resetRightI();
#endif
		GetWatchdog().Feed();
		myfile<<count<<","<<drive->getL()<<"\n";
		Wait(0.02);
	}
	shooter->closeFile();
	myfile.close();
}

void PewPewBot::Disabled()
{
	//drive->resetEncoders();
#if SHOOTER_PID
	shooter->setEnabled(false);
#endif
	//close file
#if DRIVE_PID
	drive->cleanPID();
#endif
	while (IsDisabled())
	{
		Wait(0.02);
	}
}

START_ROBOT_CLASS(PewPewBot)
;
