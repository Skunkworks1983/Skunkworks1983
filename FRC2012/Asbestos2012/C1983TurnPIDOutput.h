#include "WPILib.h"

class C1983TurnPIDOutput : public PIDOutput
{
public:
	C1983TurnPIDOutput(Victor *lvic1, Victor *lvic2, Victor *rvic1, Victor *rvic2);
	void PIDWrite(float output);
	
private:
	Victor *vicLeft1,*vicLeft2,*vicRight1,*vicRight2;
};
