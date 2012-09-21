#include "C1983Tipper.h"
C1983Tipper::C1983Tipper()
{
	tipperUp = new Solenoid(TIPPER_UP_CHANNEL);
	tipperDown = new Solenoid(TIPPER_DOWN_CHANNEL);
	isDown = false;
}

/*
 * Created: January 23, 2012
 * By: Quinn Schiller
 * 
 * Last Updated: January 24, 2012
 * By: Quinn Schiller
 * 
 */

/* Raises and lowers 
 * allows crossing of the bridge
 * grow as a person
 */
void C1983Tipper::tip(bool down)
{
	//lower the tipper
	if(down && !isDown)
	{
		tipperUp->Set(false);
		tipperDown->Set(true);
		isDown = true;
	}else if(!down && isDown){
		tipperUp->Set(true);
		tipperDown->Set(false);
		isDown = false;
	}else{
		return;
	}
};

//Gets state
bool C1983Tipper::getState()
{
	return isDown;
};

