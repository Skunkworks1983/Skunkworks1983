#ifndef __C1983TIPPER_H
#define __C1983TIPPER_H
#include "1983Defines2012.h"
#include "WPILib.h"

/*
 * Created: January 23, 2012
 * By: Quinn Schiller
 * 
 * Last Updated: January 24, 2012
 * By: Quinn Schiller
 * 
 */

class C1983Tipper
{
private:
	Solenoid *tipperUp;
	Solenoid *tipperDown;
	bool isDown;

public:
	C1983Tipper();
	
	// Raise and lower the tipper
	void tip(bool down);
	
	//returns the state of tipper
	bool getState();
	
	//END THE MADNESS
	~C1983Tipper();
	
};
#endif
