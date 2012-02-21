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
}

PewPewBot::~PewPewBot()
{
}

void PewPewBot::OperatorControl()
{
	DriverStationEnhancedIO &myEIO = driverStation->GetEnhancedIO();
	int count = 0;//DEBUG (C1983)
	int tipperToggle = 0;
	int shifterToggle = 0;
#if DRIVE_PID
	drive->enablePID();
#endif
	//drive->turnPID->Enable();
	while (IsOperatorControl() && !IsDisabled())
	{
		tipperToggle--;
		shifterToggle--;
		
		count++;
		if (count/25 == (float)count/25)
		{
			//cout<<"Low(9): "<<collector->getSense(0)<<" Mid(7): "<<collector->getSense(1)<<" Top(8): "<<collector->getSense(2)<<endl;
			//cout<<"SpeedL: "<<drive->getL()<<" SpeedR: "<<drive->getR()<<" Throttle: "<<rStick->GetThrottle()<<" Shooter power: "<<(rStick->GetThrottle() + 1)/2<<endl;
			//cout<<"Setpoint: "<<drive->turnPID->GetSetpoint()<<" Angle: "<<drive->getGyro()<<" Error: "<<drive->turnPID->GetError()<<endl;
			//cout<<"SpeedL: "<<drive->getL()<<"SetpointL: "<<drive->getLSetpoint()<<" ErrorL: "<<drive->getLError()<<" LeftP: "<<drive->getP()<<endl;
			cout<<"LSpeed: "<<drive->getL()<<" RSpeed: "<<drive->getR()
					<<" Speed/Ideal Max: "<<drive->getL()/MAXSPEEDHIGH
					<< "Shooter Power: "<<-(rStick->GetThrottle() + 1)/2<<endl;
		}

		//Set the compressor
		drive->updateCompressor();

		//Set the drive base to the stick speeds (Joysticks are backwards yo!)
		if (fabs(rStick->GetY()) >= DEADBAND)
		{
			drive->setSpeedR((-rStick->GetY()) * (-rStick->GetY())
					* (-rStick->GetY()));
		} else
		{
			drive->setSpeedR(0.0);
		}
		if (fabs(lStick->GetY()) >= DEADBAND)
		{
			drive->setSpeedL((-lStick->GetY()) * (-lStick->GetY())
					* (-lStick->GetY()));
		} else
		{
			drive->setSpeedL(0.0);
		}

		//check for shifting
		if (lStick->GetRawButton(1) && shifterToggle <= 0)
		{
			cout << "Toggle shift" << endl;
			drive->shift(!(drive->shiftedHigh));
			shifterToggle = 20;
		}

		//COLLECTOR
		if (COLLECT_BUTTON)
		{
			collector->requestCollect();
		}
		collector->update();

		//SHOOTER
		shooter->setJankyPower(rStick->GetThrottle());

		//Check for shot
		if (SHOOT_BUTTON)
		{
			collector->requestShot();
		}
		//Tipper
		if (TIPPER_SWITCH && tipperToggle < 0)
		{
			cout << "Toggle tipper" << endl;
			//drive->toggleTipper()
			tipperToggle = 20;
			drive->tip(!(drive->tipper->getState()));
		}
		//check for light
		if (rStick->GetRawButton(1))
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
		Wait(0.01);
	}
}

int PewPewBot::getOperatorControlMode()
{

	return 0;
}

void PewPewBot::Disabled()
{
	drive->resetEncoders();
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
