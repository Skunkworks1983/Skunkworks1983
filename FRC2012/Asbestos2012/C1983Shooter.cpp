#ifndef __C1983Shooter_
#define __C1983Shooter_
#include "C1983Shooter.h"
#if SHOOTER

C1983Shooter::C1983Shooter()
{
	shooterEncoder = new Encoder(SHOOTER_WHEEL_ENCODER_A,SHOOTER_WHEEL_ENCODER_B,false,Encoder::k4X);
	shooterVic1 = new Victor(SHOOTER_VIC_CHANNEL1);
	shooterVic2 = new Victor(SHOOTER_VIC_CHANNEL2);
	//hoodAngler = new Relay(SHOOTER_HOOD_CHANNEL);
	shooterEncoder->SetPIDSourceParameter(Encoder::kRate);
	shooterEncoder->SetDistancePerPulse(SHOOTER_UNITS_PER_TICK);
	shooterEncoder->Start();
	shooterEncoder->Reset();
	shooterPIDOutput = new C1983PIDOutput(shooterVic1,shooterVic2,true);
	shooterPIDSource = new C1983PIDSource(shooterEncoder,3200.0,false);
	shooterPID = new PIDController(SHOOTER_P,SHOOTER_I,SHOOTER_D,shooterEncoder,shooterPIDOutput);
	shooterPID->SetInputRange(-1.0,1.0);
	shooterPID->SetOutputRange(-1.0,1.0);
	power = SHOT_LAYUP_SPEED;
}

void C1983Shooter::setShot(short shotNum)
{
	switch(shotNum)
	{
	case 0:
		setPower(SHOT_LAYUP_SPEED);
		setAngle(SHOT_LAYUP_ANGLE);
		break;
	case 1:
		setPower(SHOT_FREETHROW_SPEED);
		setAngle(SHOT_FREETHROW_ANGLE);
		break;
	case 2:
		setPower(SHOT_OTHER_SPEED);
		setAngle(SHOT_OTHER_SPEED);
	}
}

void C1983Shooter::setPower(float powerRPM)
{
	power = powerRPM;
	shooterPID->SetSetpoint(power);
}

void C1983Shooter::setAngle(bool high)
{
	if(high)
	{
		hoodAngler->Set(HOOD_HIGH);
	}else{
		hoodAngler->Set(HOOD_LOW);
	}
}

double C1983Shooter::getRate()
{
	return shooterEncoder->GetRate();
}

bool C1983Shooter::isReady()
{
	return true;
	//return fabs(shooterEncoder->GetRate() - goalRPM) <= SHOOTER_VELOCITY_TOLERANCE;
}

void C1983Shooter::setOn(bool on)
{
#if SHOOTER_PID
	if(!on)
	{
		shooterPID->SetSetpoint(0.0);
		active = false;
	}else{
		shooterPID->SetSetpoint(power);
		active = true;
	}
#else
	active = on;
#endif
}

void C1983Shooter::setJankyPower(float power)
{
	if(active)
	{
		shooterVic1->Set(-power);
		shooterVic2->Set(-power);
	}else{
		jankyStop();
	}
}

void C1983Shooter::jankyStop()
{
	shooterVic1->Set(0.0);
	shooterVic2->Set(0.0);
}

void C1983Shooter::printShit()
{
	cout<<" Actual: "<<shooterEncoder->GetRate();
}

void C1983Shooter::setEnabled(bool enabled)
{
	if(enabled)
	{
		shooterPID->Enable();
	}else{
		shooterPID->Disable();
	}
}

bool C1983Shooter::isOn()
{
	return active;
}
#endif
#endif
