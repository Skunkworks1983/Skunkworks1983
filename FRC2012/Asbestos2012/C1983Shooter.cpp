#ifndef __C1983Shooter_
#define __C1983Shooter_
#include "C1983Shooter.h"
#if SHOOTER

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
	shooterPIDSource = new C1983ShooterPIDSource(shooterEncoder,3300.0,true);
	shooterPID = new PIDController(SHOOTER_P,SHOOTER_I,SHOOTER_D,shooterPIDSource,shooterPIDOutput);
	shooterPID->SetInputRange(-1.0,1.0);
	shooterPID->SetOutputRange(-1.0,1.0);
	shooterPID->Disable();
#endif
	isEnabled = false;
	manual = false;
	power = SHOT_FREETHROW_SPEED;

}

void C1983Shooter::setShot(short shotNum)
{
#if SHOOTER_PID
	switch(shotNum)
	{
		case kKeytop:
		setPower(SHOT_KEYTOP_SPEED/SHOOTER_MAX_SPEED);
		break;
		case kFreethrow:
		setPower(SHOT_FREETHROW_SPEED/SHOOTER_MAX_SPEED);
		break;
		case 2:
		setPower(SHOT_OTHER_SPEED/SHOOTER_MAX_SPEED);
	}
#endif
}

#if SHOOTER_PID
void C1983Shooter::setPower(float powerRPM)
{
	power = powerRPM;
	if(power == 0.0)
	{
		shooterPID->Reset();
		shooterPID->Enable();
		shooterPID->SetSetpoint(0.0);
	}else{
		shooterPID->SetSetpoint(power);
	}
}
#endif

bool C1983Shooter::isReady()
{
	if(manual)
		return true;
	else
		return fabs(shooterPIDSource->PIDGet() - shooterPID->GetSetpoint())*SHOOTER_MAX_SPEED <= SHOOTER_VELOCITY_TOLERANCE;
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
	if(enable && !isEnabled)
	{
		shooterPID->Enable();
		shooterPID->SetSetpoint(power);
		cout<<"ENABLING SHOOTER PID"<<endl;
		isEnabled = true;
	} else if(!enable && isEnabled){
		cout<<"DISABLING SHOOTER PID"<<endl;
		shooterPID->SetSetpoint(0.0);
		shooterPID->Disable();
		isEnabled = false;
	}else{
		return;
	}
#endif
}

void C1983Shooter::update()
{
#if SHOOTER_PID
	shooterPIDSource->updateAverage();
#endif
}

bool C1983Shooter::getEnabled()
{
	return isEnabled;
}

#if SHOOTER_PID
void C1983Shooter::cleanPID()
{
	shooterPID->Reset();
	shooterPID->Enable();
}
#endif
#endif
#endif
