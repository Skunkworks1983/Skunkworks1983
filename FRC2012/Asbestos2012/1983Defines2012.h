#ifndef __1983DEFINES_H
#define __1983DEFINES_H

//PewPew Start
#define KINECT 0
#define USE_PID 1

//PewPew End

//DriveBase Start
//Air Compressor
#define COMPRESSOR_SWITCH_PORT 14
#define DIGITAL_MODULE 1
#define COMPRESSOR_PORT 8

//Jaguars
#define JAG_PORT_LEFT_1 1
#define JAG_PORT_LEFT_2 2
#define JAG_PORT_RIGHT_1 3
#define JAG_PORT_RIGHT_2 4

//Encoders
#define LEFT_ENCODER_PORT_A 6
#define LEFT_ENCODER_PORT_B 7
#define RIGHT_ENCODER_PORT_A 4
#define RIGHT_ENCODER_PORT_B 5

//PIDs
#define DRIVE_P 3.5
#define DRIVE_I 0.0//0.1
#define DRIVE_D 0

#define DRIVE_P_LOW 2.3
#define DRIVE_I_LOW 0
#define DRIVE_D_LOW 0

#define LIGHT_SENSOR_CHANNEL_FRONT 11
#define LIGHT_SENSOR_CHANNEL_BACK 13
#define LIGHT_CHANNEL 1

//Max speeds for the wheels in low and high gears
#define MAXSPEEDHIGH 43
#define MAXSPEEDLOW  13

#define LINE_STOP_SPEED .25 //The speed at which the robot runs to stop at the key
//DriveBase End

//Shooter Start
#define SHOOTER_WHEEL	0				//Channel for shooter wheel
#define SHOOTER_HOOD 	0				//Channel for shooter hood

#define SHOOTER_HOOD_POT		0		//Channel of the hood measurment pot	
#define SHOOTER_WHEEL_ENCODER_A 0		//Channels for the shooter wheel's encoder
#define SHOOTER_WHEEL_ENCODER_B 0		

										//Accuracy tolerance.  How close the speed/position has to get to be accurate
#define SHOOT_VELOCITY_TOLERANCE	0	//Tolerance of the velocity
#define SHOOT_ANGLE_TOLERANCE 		0	//Tolerance of the angle

//SHooter PIDs
#define SHOOTER_WHEEL_P 0
#define SHOOTER_WHEEL_I 0
#define SHOOTER_WHEEL_D 0

#define SHOOTER_HOOD_P 0
#define SHOOTER_HOOD_I 0
#define SHOOTER_HOOD_D 0

//Preset shot speeds
#define SHOT_LAYUP_SPEED 0.0
#define SHOT_LAYUP_ANGLE 0.0

#define SHOT_FREETHROW_SPEED 0.0
#define SHOT_FREETHROW_ANGLE 0.0

#define SHOT_OTHER_SPEED 0.0
#define SHOT_OTHER_ANGLE 0.0

#define BALL_SPEED_TO_RPM(speed) {return speed;}   //TODO Conversion
//Shooter End

//Tipper Start
#define TIPPER_PORT 5
//Tipper End

//Collector Start
#define COLLECTOR_VIC_PICKUP 0
#define COLLECTOR_VIC_BOTTOM 0
#define COLLECTOR_VIC_TOP 0
//Number of sensed ball storage spots
#define COLLECTOR_SLOT_COUNT 3
//Belt Vic Speed
#define COLLECTOR_BELT_SPEED 1.0
//Collector Vic Speed
#define PICKUP_SPEED 1.0

//Collector IR Sensors
#define COLLECTOR_IR_LOW_CHANNEL 0
#define COLLECTOR_IR_TOP_CHANNEL 0
//Collector End

//GlyphCamera Start
#define GLYPH_CAMERA_IP "10.19.83.4"

//Low bound of threshold
#define GLYPH_THRESH_R_L 200
#define GLYPH_THRESH_G_L 200
#define GLYPH_THRESH_B_L 200

//High bound of threshold
#define GLYPH_THRESH_R_H 255
#define GLYPH_THRESH_G_H 255
#define GLYPH_THRESH_B_H 255
//GlyphCamera End

//Controls Begin
#define SHIFT_BUTTON lStick->GetRawButton(1);

#endif
