#if 0
#include "C1983Shooter.h"
#include "Victor.h"

C1983Shooter::C1983Shooter() {
	//Create Motor Controllers
	shooterVic = new Victor(SHOOTER_WHEEL);
	hoodVic = new Victor(SHOOTER_HOOD);

	//Create sensors
	hoodPot = new AnalogChannel(SHOOTER_HOOD_POT);
	wheelEncoder = new Encoder(1,SHOOTER_WHEEL_ENCODER_A,1, SHOOTER_WHEEL_ENCODER_B);
	wheelEncoder->Start();

	//Create PIDs
	hoodPID = new PIDController(SHOOTER_HOOD_P,SHOOTER_HOOD_I,SHOOTER_HOOD_D,hoodPot,hoodVic);
	wheelPID
			= new PIDController(SHOOTER_WHEEL_P,SHOOTER_WHEEL_I,SHOOTER_WHEEL_D,wheelEncoder,shooterVic);
}

void C1983Shooter::setVelocity(float velocity) {
	targetVelocity = velocity;
	wheelPID->SetSetpoint(velocity);
}

void C1983Shooter::setAngle(float angle) {
	targetAngle = angle;
	hoodPID->SetSetpoint(targetAngle);
}

double C1983Shooter::getVelocity() {
	return wheelEncoder->GetRate(); //TODO configure the encoder for correct values
}

float C1983Shooter::getAngle() {
	return hoodPot->GetVoltage(); //TODO Calibrate to angle value instead of voltage
}

bool C1983Shooter::isReady() {
	float velDiff = getVelocity() - targetVelocity; //Velocity Difference
	velDiff = velDiff<0 ? -velDiff : velDiff; //abs
	float angDiff= getAngle()- targetAngle; //Angle Difference
	angDiff = angDiff<0 ? -angDiff : angDiff; //abs
	return velDiff <= SHOOT_VELOCITY_TOLERANCE && angDiff
			<= SHOOT_ANGLE_TOLERANCE;
}

void C1983Shooter::aim(int preset = kLayup) {
	switch (preset) {
	case kLayup:
		setAngle(SHOT_LAYUP_ANGLE);
		setVelocity(
		SHOT_LAYUP_SPEED);
		break;
	case kFreeThrow:
		setAngle(SHOT_FREETHROW_ANGLE);
		setVelocity(
		SHOT_FREETHROW_SPEED);
		break;
	case kOther:
		setAngle(SHOT_OTHER_ANGLE);
		setVelocity(
		SHOT_OTHER_SPEED);
		break;
	}
}
#endif
