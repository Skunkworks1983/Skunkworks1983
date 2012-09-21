#include "C1983Shooter.h"

/*
 * 
 if (enabled)
 {
 float speed = shooterPIDSource->PIDGet();
 float curr = shooterVic1->Get();
 if (speed < power)
 {
 curr+=SHOOTER_CHANGE_LIMIT;
 if (curr > 1)
 {
 curr = 1;
 }
 } else if (speed >= power)
 {
 curr = 0;
 }
 cout << speed << "," << curr << "," << power << endl;
 setJankyPower(curr);
 } else
 {
 jankyStop();
 }
 */

C1983Shooter::C1983Shooter()
{
	shooterEncoder = new Encoder(SHOOTER_WHEEL_ENCODER_A,SHOOTER_WHEEL_ENCODER_B,false,Encoder::k1X);
	shooterVic1 = new Victor(SHOOTER_VIC_CHANNEL1);
	shooterVic2 = new Victor(SHOOTER_VIC_CHANNEL2);
	shooterEncoder->SetPIDSourceParameter(Encoder::kRate);
	shooterEncoder->SetDistancePerPulse(SHOOTER_UNITS_PER_TICK);
	shooterEncoder->Start();
#if SHOOTER_PID
	shooterPIDSource = new C1983ShooterPIDSource(shooterEncoder,SHOOTER_MAX_SPEED,true);
	shooterPIDOutput = new C1983ShooterPIDOutput(shooterVic1,shooterVic2,true,shooterPIDSource);
	shooterPID
			= new PIDController(SHOOTER_P,SHOOTER_I,SHOOTER_D,shooterPIDSource,shooterPIDOutput, 0.01);
	shooterPID->SetInputRange(-1.0, 1.0);
	shooterPID->SetOutputRange(-1.0, 1.0);
	shooterPID->Disable();
#if SHOOTER_BANGBANG
	m_semaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
	m_controlLoop = new Notifier(C1983Shooter::callBangBang, this);
#endif
#endif
	manual = false;
	power = SHOT_FREETHROW_SPEED;
	currentShot = kFreethrow;
	stableReady = 0;
	//File stuff
	fileNumber = 0;
	fileIndex = 0;
	fileOpen = false;

	pLow = SHOOTER_P;
	iLow = SHOOTER_I;
	dLow = SHOOTER_D;

	pHigh = SHOOTER_P_HIGH;
	iHigh = SHOOTER_I_HIGH;
	dHigh = SHOOTER_D_HIGH;

	enabled = false;
#if SHOOTER_PID
	readPIDFromFile();
#endif
#if SHOOTER_BANGBANG
	m_controlLoop->StartPeriodic(0.01);
#endif
}

void C1983Shooter::callBangBang(void * inst)
{
	((C1983Shooter*)inst)->bangBang();
}

void C1983Shooter::bangBang()
{
	shooterPIDOutput->PIDWrite(enabled);
}
void C1983Shooter::setShot(short shotNum)
{
#if SHOOTER_PID
	currentShot = shotNum;
	switch (shotNum)
	{
	case kKeytop:
		setPower(SHOT_KEYTOP_SPEED/SHOOTER_MAX_SPEED);
		shooterPID->SetPID(pHigh, iHigh, dHigh);
		break;
	case kFreethrow:
		setPower(SHOT_FREETHROW_SPEED/SHOOTER_MAX_SPEED);
		shooterPID->SetPID(pLow, iLow, dLow);
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
#if !SHOOTER_BANGBANG
		shooterPID->Enable();
#endif
		enabled = true;
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
	if (power != 0 && (!shooterPID->IsEnabled() || shooterPID->GetSetpoint()
			== 0))
		return false;
	float error = (shooterPIDSource->getRealAverage() - shooterPID->GetSetpoint())
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
	shooterVic1->Set(-power);
	shooterVic2->Set(-power);
}

void C1983Shooter::jankyStop()
{
	shooterVic1->Set(0.0);
	shooterVic2->Set(0.0);
}

void C1983Shooter::setEnabled(bool enable)
{
#if SHOOTER_PID
	if (enable && !shooterPID->IsEnabled())
	{
#if !SHOOTER_BANGBANG
		shooterPID->Enable();
#endif
		shooterPID->SetSetpoint(power);
		enabled = true;
		//cout<<"ENABLING SHOOTER PID"<<endl;
	} else if (!enable && shooterPID->IsEnabled())
	{
		//cout<<"DISABLING SHOOTER PID"<<endl;
		shooterPID->SetSetpoint(0.0);
#if !SHOOTER_BANGBANG
		shooterPID->Reset();
		shooterPID->Disable();
#endif
		enabled = false;
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
	setShot(currentShot);
}

double C1983Shooter::getPIDAdjust()
{
	return PIDMod;
}
#endif

void C1983Shooter::update()
{
#if SHOOTER_PID
	shooterPIDOutput->setSetpoint(shooterPID->GetSetpoint());
#endif

	//Update stability
	if (isReady())
	{
		stableReady++;
	} else
	{
		stableReady = 0;
	}
}

bool C1983Shooter::getEnabled()
{
	return shooterPID->IsEnabled();
}

#if SHOOTER_PID
void C1983Shooter::readPIDFromFile()
{
	string line;
	ifstream myfile("/PID.txt");
	if (myfile.is_open())
	{
		for (int i = 0; i<=1 && myfile.good(); i++)
		{
			getline(myfile, line);
			float P = -1, I = -1, D = -1;
			sscanf(&(line[0]), "%f,%f,%f", &P, &I, &D);
			switch (i)
			{
			case 0:
				pLow = P != -1 ? P : pLow;
				iLow = I != -1 ? I : iLow;
				dLow = D != -1 ? D : dLow;
				break;
			case 1:
				pHigh = P != -1 ? P : pHigh;
				iHigh = I != -1 ? I : iHigh;
				dHigh = D != -1 ? D : dHigh;
				break;
			}
		}
		myfile.close();
	} else
	{
		cout << "Unable to read PID values from file!  Creating!" << endl;
		ofstream out("/PID.txt");
		if (out.is_open())
		{
			out << pLow << "," << iLow << "," << dLow << " #LOW PID" << endl;
			out << pHigh << "," << iHigh << "," << dHigh << " #HIGH PID"
					<< endl;
			out.close();
		}
	}
}
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
