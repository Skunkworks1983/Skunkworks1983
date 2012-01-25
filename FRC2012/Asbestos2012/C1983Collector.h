#ifndef __C1983COLLECTOR_H
#define __C1983COLLECTOR_H
#include "WPIlib.h"
#include "1983Defines2012.h"

class C1983Collector
{

private:		
		Jaguar *collectorJag1; //Jaguars for the shooting wheels
		Jaguar *collectorJag2;
		Jaguar *collectorJag3;
		Jaguar *collectorJag4;

		Encoder *Jag1encoder;
		Encoder *fakeEncoder1;
		Encoder *Jag2encoder;
		Encoder *fakeEncoder2;
		Encoder *Jag3encoder;
		Encoder *fakeEncoder3;
		Encoder *Jag4encoder;
		Encoder *fakeEncoder4;


public:

		// Reads if a ball has entered the conveyor.
		bool BallEnter();

		//Feeds the nall through the collector.
		void feed();

		// Check if ball is in shooting position.
		void isBallReadyShoot();

		//Check if there is a ball ready to enter the shooting chamber of the collector.
		void isBallReadyStage();

		// Checks the amount of balls that has entered the collector.
		int getBallCount();

		void setBottom();
		void setTop();
};
#endif
