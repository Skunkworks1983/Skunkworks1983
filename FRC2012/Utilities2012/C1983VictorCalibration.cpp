#include "C1983VictorCalibration.h"

C1983VictorCalibration::C1983VictorCalibration()
{
	stick = new Joystick(1);
	victor = new Victor(1);
}

void C1983VictorCalibration::OperatorControl()
{
	bool toggle = false;
	bool backToggle = false;
	int count = 0;
	while (IsOperatorControl() && !IsDisabled())
	{
		count++;
		if (count % 2000)
		{
			cout << "Current Channel: " << victor->GetChannel()
					<< "\tSpeed" << victor->Get() << endl;
		}
		if (!toggle && (toggle = lStick->GetRawButton(5)))
		{
			int channel = victor->GetChannel();
			victor->Set(0);
			delete victor;
			victor = new Victor(channel+1)
		} else
		{
			toggle = lStick->GetRawButton(5);
		}
		if (!backToggle && (backToggle = lStick->GetRawButton(4))
				&& victor->GetChannel() > 1)
		{
			int channel = victor->GetChannel();
			victor->Set(0);
			delete victor;
			victor = new Victor(channel-1)
		} else
		{
			backToggle = lStick->GetRawButton(4);
		}
		victor->Set(lStick->GetY());
		Wait(0.02);
	}
}