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
#define DRIVE_P 1.5
#define DRIVE_I 0.1
#define DRIVE_D 0

//Max speeds for the wheels in low and high gears
#define MAXSPEEDHIGH 51
#define MAXSPEEDLOW  10
//DriveBase End

//Shooter Start
#define SHOOTER_WHEEL 0
#define SHOOTER_HOOD 0
#define SHOOT_VELOCITY_TOLERANCE 0
#define SHOOT_ANGLE_TOLERANCE
//Tipper End


//Tipper Start
#define TIPPER_PORT 5
//Tipper End

//Collector Start
#define COLLECTOR_FEED_VIC 0
#define COLLECTOR_VIC_BOTTOM 0
#define COLLECTOR_VIC_TOP 0
//Number of sensed ball storage spots
#define COLLECTOR_SLOT_COUNT 3
//Belt speed
#define COLLECTOR_BELT_SPEED 1.0
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

#define COLLECTORDRIVESPEED = 1;

#endif
