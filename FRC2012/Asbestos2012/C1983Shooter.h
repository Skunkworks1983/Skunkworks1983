#ifndef __C1983Shooter_H
#define __C1983Shooter_H
#include "WPILib.h"
#include "math.h"
#include "1983Defines2012.h"
#include "C1983ShooterPIDOutput.h"
#include "C1983Encoder.h"
#include "C1983ShooterPIDSource.h"
#include <fstream>
#include <sstream>
class C1983Shooter
{
private:
	Victor *shooterVic1;
	Victor *shooterVic2;
#if SHOOTER_PID
	C1983ShooterPIDOutput *shooterPIDOutput;
	C1983ShooterPIDSource *shooterPIDSource;
	PIDController *shooterPID;
#endif
#if SHOOTER_BANGBANG
	SEM_ID m_semaphore;
	Notifier *m_controlLoop;
#endif
	//C1983Encoder *shooterEncoder;
	Encoder *shooterEncoder;

	bool enabled;
	float power;
	bool manual;
	short currentShot;
	double PIDMod;
	//File stuff
	int fileNumber;
	ofstream *data;
	float fileIndex;
	bool fileOpen;
	//End file stuff
	int stableReady;
	float pLow, iLow, dLow, pHigh, iHigh, dHigh;

public:
	C1983Shooter();
	bool isReady();
	bool isStableReady();
	double getRate();
	bool getEnabled();
	void setShot(short shotNum);
	void setJankyPower(float power);
	void jankyStop();
	void debugPrint();
	void setEnabled(bool enabled);
	char * getShotName();
	static void callBangBang(void * inst);
	void bangBang();

#if SHOOTER_PID
	void setPower(float powerRPM);
	void cleanPID();
	void dUp();
	void dDown();
	void pUp();
	void pDown();
	void iUp();
	void iDown();
	void setPIDAdjust(double adjust);
	double getPIDAdjust();

	float getError();
	float getI();
	float getP();
	float getD();
	float getSetpoint();
	double getPercent();
#endif
	//file stuff
	char* getFileName();
	void openFile();
	void closeFile();
	bool getIsOpen();
	void writeFile();
	void readPIDFromFile();
	//end file stuff
	void update();
	enum
	{	kKeytop, kFreethrow, kOther} shot;
};
#endif
