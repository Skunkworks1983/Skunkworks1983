#include "Bot.h"

/*double timediff ()
{
    static struct timespec ts = {0,0};
    struct timespec tt;
    clock_gettime (CLOCK_REALTIME, &tt);
    double diff = ((double) tt.tv_sec - ts.tv_sec) +
            (tt.tv_nsec - ts.tv_nsec)/1000000000.0;
    clock_gettime (CLOCK_REALTIME, &ts);
    return diff;
}*/

Bot::Bot()
{
	shooterVic1 = new Victor(8);
	shooterVic2 = new Victor(9);
}

void Bot::OperatorControl()
{
	encoder = new C1983Encoder(10);
	while(IsOperatorControl() && !IsDisabled())
	{
		shooterVic1->Set(-0.10);
		shooterVic2->Set(-0.10);
		//cout<<timediff()<<"\t"<<encoder->Get()<<endl;
	}
}

void Bot::Autonomous(){}

void Bot::Disabled(){}

Bot::~Bot(){};

START_ROBOT_CLASS(Bot);
