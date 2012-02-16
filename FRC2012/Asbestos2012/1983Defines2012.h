#ifndef __1983DEFINES_H
#define __1983DEFINES_H

//PewPew Start
#define KINECT 0
#define DEADBAND 0.05

//PewPew End

//DriveBase Start
//Air Compressor
#define COMPRESSOR_SWITCH_PORT 4
#define DIGITAL_MODULE 1
#define COMPRESSOR_PORT 1

//Drive Victors
#define VIC_PORT_LEFT_1 1
#define VIC_PORT_LEFT_2 2
#define VIC_PORT_RIGHT_1 3
#define VIC_PORT_RIGHT_2 4

//Encoders
#define LEFT_ENCODER_PORT_A 11
#define LEFT_ENCODER_PORT_B 12
#define RIGHT_ENCODER_PORT_A 13
#define RIGHT_ENCODER_PORT_B 14

//PIDs
#define DRIVE_P 2.8
#define DRIVE_I 0.06//0.1
#define DRIVE_D 0

#define DRIVE_P_LOW 2.3
#define DRIVE_I_LOW 0.0
#define DRIVE_D_LOW 0

#define TURN_P 0.05
#define TURN_I 0.0
#define TURN_D 0.0

#define TURN_P_LOW 2.3
#define TURN_I_LOW 0.0
#define TURN_D_LOW 0.0

#define LIGHT_SENSOR_CHANNEL_FRONT 1
#define LIGHT_SENSOR_CHANNEL_BACK 2
#define LIGHT_SENSOR_CHANNEL_BRIDGE 3
#define LIGHT_CHANNEL 1

//Gyro Channel	
#define GYRO_CHANNEL 1

//Max speeds for the wheels in low and high gears
#define MAXSPEEDHIGH 43
#define MAXSPEEDLOW  13

#define LINE_STOP_SPEED .25 //The speed at which the robot runs to stop at the key
//DriveBase End

//Shooter Start
#define SHOOTER 1

#define SHOOTER_VIC_CHANNEL 0				//Channel for shooter wheel
#define SHOOTER_HOOD_CHANNEL 0

#define SHOOTER_WHEEL_ENCODER_A 1		//Channels for the shooter wheel's encoder
#define SHOOTER_WHEEL_ENCODER_B 2		

										//Accuracy tolerance.  How close the speed/position has to get to be accurate
#define SHOOTER_VELOCITY_TOLERANCE	0	//Tolerance of the velocity

//SHooter PIDs
#define SHOOTER_P 0
#define SHOOTER_I 0
#define SHOOTER_D 0

//Preset shot speeds
#define SHOT_LAYUP_SPEED 0.0
#define SHOT_LAYUP_ANGLE true

#define SHOT_FREETHROW_SPEED 0.0
#define SHOT_FREETHROW_ANGLE true

#define SHOT_OTHER_SPEED 0.0
#define SHOT_OTHER_ANGLE true

#define BALL_SPEED_TO_RPM(speed) {return speed;}   //TODO Conversion

#define HOOD_HIGH Relay::kReverse
#define HOOD_LOW Relay::kForward

#define SHOT_AWAY_SWITCH 0
//Shooter End

//Tipper Start
#define TIPPER_PORT 5
//Tipper End

//Collector Start
#define COLLECTOR_VIC_PICKUP 0
#define COLLECTOR_VIC_LOW 0
#define COLLECTOR_VIC_TOP 0
//Number of sensed ball storage spots
#define COLLECTOR_SLOT_COUNT 3
//Belt Vic Speed
#define COLLECTOR_BELT_SPEED 0.2
//Collector Vic Speed
#define COLLECTOR_PICKUP_SPEED 0.2
//Timeout for blind running collectors
#define COLLECTOR_TIMEOUT 150

//Collector IR Sensors
#define COLLECTOR_IR_LOW_CHANNEL 0
#define COLLECTOR_IR_MID_CHANNEL 0
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
#define COLLECT_BUTTON
#define SHOOT_BUTTON
#define ARM_BUTTON
#define MANUAL_BUTTON
#define MANUAL_SWITCH
#define FORWARD_SWITCH
#define TIPPER_SWITCH
#define COLLECT_BUTTON_OP //operator's collect button
#define LAYUP_BUTTON
#define FREETHROW_BUTTON
#define RPM_MANUAL_SLIDER
#define KEY_ALIGN_BUTTON
#define AUTO_TARGER_BUTTON
#define FULL_AUTO_SWTICH


#endif
