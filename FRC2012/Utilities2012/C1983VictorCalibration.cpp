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
		if (!toggle && (toggle = stick->GetRawButton(5)))
		{
			int channel = victor->GetChannel();
			victor->Set(0);
			delete victor;
			victor = new Victor(channel+1);
		} else
		{
			toggle = stick->GetRawButton(5);
		}
		if (!backToggle && (backToggle = stick->GetRawButton(4))
				&& victor->GetChannel() > 1)
		{
			int channel = victor->GetChannel();
			victor->Set(0);
			delete victor;
			victor = new Victor(channel-1);
		} else
		{
			backToggle = stick->GetRawButton(4);
		}
		victor->Set(stick->GetY());
		Wait(0.02);
	}
}