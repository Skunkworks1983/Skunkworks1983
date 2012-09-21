#ifndef PEW_UTILITY
#define PEW_UTILITY
#include "WPILib.h"

//Timejunk
class System
{
public:
	static double currentTimeMillis()
	{
		struct timespec tt;
		clock_gettime(CLOCK_REALTIME, &tt);
		double diff = (tt.tv_sec*1000.0) + (double)(tt.tv_nsec /1000000.0);
		return diff;
	}
};
#endif
