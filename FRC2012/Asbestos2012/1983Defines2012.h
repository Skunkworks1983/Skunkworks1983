#ifndef __1983DEFINES_H
#define __1983DEFINES_H

//DriveBase Start
//Jaguars
#define JAG_PORT_LEFT_1 1
#define JAG_PORT_LEFT_2 2
#define JAG_PORT_RIGHT_1 3
#define JAG_PORT_RIGHT_2 4

//Encoders
#define LEFT_ENCODER_PORT_A 1
#define LEFT_ENCODER_PORT_B 2
#define RIGHT_ENCODER_PORT_A 3
#define RIGHT_ENCODER_PORT_B 4

//PIDs
#define DRIVE_P 0
#define DRIVE_I 0
#define DRIVE_D 0
//DriveBase End

//Shooter Start
//The target RPM of the shooter wheel
#define TARGET_SHOOT_VELOCITY 0
//Tolerance of the velocity
#define SHOOT_VELOCITY_TOLERANCE 10
//Shooter End

//Tipper Start
#define TIPPER_PORT 5

//Collector Start
#define COLLECTORJAG1PORT 0
#define COLLECTORJAG2PORT 0
#define COLLECTORJAG3PORT 0
#define COLLECTORJAG4PORT 0
//Number of sensed ball storage spots
#define COLLECTOR_SLOT_COUNT 3
//Collector End

#endif
