#include "C1983Shooter.h"

C1983Shooter::C1983Shooter()
{
	shooterEncoder = new Encoder(SHOOTER_WHEEL_ENCODER_A,SHOOTER_WHEEL_ENCODER_B,false,Encoder::k1X);
	shooterVic1 = new Victor(SHOOTER_VIC_CHANNEL1);
	shooterVic2 = new Victor(SHOOTER_VIC_CHANNEL2);
	shooterEncoder->SetPIDSourceParameter(Encoder::kRate);
	shooterEncoder->SetDistancePerPulse(SHOOTER_UNITS_PER_TICK);
	shooterEncoder->Start();
#if SHOOTER_PID
	shooterPIDOutput = new C1983PIDOutput(shooterVic1,shooterVic2,true);
	shooterPIDSource = new C1983ShooterPIDSource(shooterEncoder,SHOOTER_MAX_SPEED,true);
	shooterPID = new PIDController(SHOOTER_P,SHOOTER_I,SHOOTER_D,shooterPIDSource,shooterPIDOutput);
	shooterPID->SetInputRange(-1.0, 1.0);
	shooterPID->SetOutputRange(-1.0, 1.0);
	shooterPID->Disable();
#endif
	isEnabled = false;
	manual = false;
	power = SHOT_FREETHROW_SPEED;
	currentShot = kFreethrow;
	stableReady = 0;
	//File stuff
	fileNumber = 0;
	fileIndex = 0;
	fileOpen = false;
}

void C1983Shooter::setShot(short shotNum)
{
#if SHOOTER_PID
	currentShot = shotNum;
	switch (shotNum)
	{
	case kKeytop:
		setPower(SHOT_KEYTOP_SPEED/SHOOTER_MAX_SPEED);
		shooterPID->SetPID(SHOOTER_P_HIGH,SHOOTER_I_HIGH,SHOOTER_D_HIGH);
		break;
	case kFreethrow:
		setPower(SHOT_FREETHROW_SPEED/SHOOTER_MAX_SPEED);
		shooterPID->SetPID(SHOOTER_P,SHOOTER_I,SHOOTER_D);
		break;
	case kOther:
		setPower(SHOT_OTHER_SPEED/SHOOTER_MAX_SPEED);
	}
#endif
}

char * C1983Shooter::getShotName()
{
	switch (currentShot)
	{
	case kKeytop:
		return "KeyTop";
	case kFreethrow:
		return "Freethrow";
	case kOther:
		return "Other";
	default:
		return "Bad Shot";
	}
}

#if SHOOTER_PID
void C1983Shooter::setPower(float powerRPM)
{
	power = powerRPM;
	if (power == 0.0)
	{
		shooterPID->Reset();
		shooterPID->Enable();
		shooterPID->SetSetpoint(0.0);
	} else
	{
		shooterPID->SetSetpoint(power * (1 + PIDMod));
	}
}
#endif

bool C1983Shooter::isReady()
{
	if (manual)
		return true;
#if SHOOTER_PID
	if (!isEnabled || shooterPID->GetSetpoint() == 0)
		return false;
	float error = (shooterPIDSource->PIDGet() - shooterPID->GetSetpoint())
			*SHOOTER_MAX_SPEED;
	return error > -SHOOTER_VELOCITY_TOLERANCE_LOW && error
			< SHOOTER_VELOCITY_TOLERANCE_HIGH;
#endif
	return true;
}

bool C1983Shooter::isStableReady()
{
	return stableReady > SHOOTER_READY_STABLITY;
}

void C1983Shooter::setJankyPower(float power)
{
	if (isEnabled)
	{
		shooterVic1->Set(-power);
		shooterVic2->Set(-power);
	} else
	{
		jankyStop();
	}
}

void C1983Shooter::jankyStop()
{
	shooterVic1->Set(0.0);
	shooterVic2->Set(0.0);
}

void C1983Shooter::setEnabled(bool enable)
{
#if SHOOTER_PID
	if (enable && !isEnabled)
	{
		shooterPID->Enable();
		shooterPID->SetSetpoint(power);
		isEnabled = true; //This is for the PID thing where the PID will technically be disabled but we're still piding
		//cout<<"ENABLING SHOOTER PID"<<endl;
	} else if (!enable && isEnabled)
	{
		//cout<<"DISABLING SHOOTER PID"<<endl;
		shooterPID->SetSetpoint(0.0);
		shooterPID->Reset();
		shooterPID->Disable();
		isEnabled = false;
	} else
	{
		return;
	}
#endif
}

#if SHOOTER_PID
void C1983Shooter::setPIDAdjust(double adjust)
{
	PIDMod = adjust;
}

double C1983Shooter::getPIDAdjust()
{
	return PIDMod;
}
#endif

void C1983Shooter::update()
{
#if SHOOTER_PID
	shooterPIDSource->updateAverage();
#endif
	/*float error;
	switch(currentShot)
	{
	case kKeytop:
		error = shooterPIDSource->PIDGet() - SHOT_KEYTOP_SPEED/SHOOTER_MAX_SPEED;
		break;
	case kFreethrow:
		error = shooterPIDSource->PIDGet() - SHOT_FREETHROW_SPEED/SHOOTER_MAX_SPEED;
		break;
	}
	if(isEnabled)
	{
		if(error < -SHOOTER_PID_RANGE)
		{
			shooterPID->Disable();
			shooterVic1->Set(-1.0);
			shooterVic2->Set(-1.0);
		}else if(error > SHOOTER_PID_RANGE){
			shooterPID->Disable();
			shooterVic1->Set(1.0);
			shooterVic2->Set(1.0);
		}else{
			shooterPID->Enable();
		}
	}*/
	
	//Update stability
	if (isReady())
	{
		stableReady++;
	} else{
		stableReady = 0;
	}
}

bool C1983Shooter::getEnabled()
{
	return isEnabled;
}

#if SHOOTER_PID
void C1983Shooter::cleanPID()
{
	bool enabled = shooterPID->IsEnabled();
	shooterPID->Reset();
	if (enabled)
		shooterPID->Enable();
	else
		shooterPID->Disable();
}
#endif
