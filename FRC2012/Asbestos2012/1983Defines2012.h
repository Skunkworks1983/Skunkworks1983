#ifndef __1983DEFINES_H
#define __1983DEFINES_H

#define PRACTICE_BOT 1

//PewPew Start
#define KINECT 0
#define DEADBAND 0.03

//PewPew End

//Autonomous Start
#define LOOP_STABILITY 10 //This define represents the amount of loops a readout must be consistently true to for the autonomous stuff to read it
//Autonomous Stop

//DriveBase Start
#define DRIVE_PID 1
//Air Compressor
#if PRACTICE_BOT
#define COMPRESSOR_SWITCH_PORT 5
#else
#define COMPRESSOR_SWITCH_PORT 4
#endif
#define DIGITAL_MODULE 1 
#define SOLENOID_MODULE 1
#define COMPRESSOR_PORT 1
#define SHIFTER_CHANNEL 1
#define SHIFTER_LOW_CHANNEL 2

//Drive Victors
#define VIC_PORT_LEFT_1 1
#define VIC_PORT_LEFT_2 2
#define VIC_PORT_RIGHT_1 3
#define VIC_PORT_RIGHT_2 4

//Encoders
#define ENCODER_SLOT 1
#define LEFT_ENCODER_PORT_A 13
#define LEFT_ENCODER_PORT_B 14
#define RIGHT_ENCODER_PORT_A 11
#define RIGHT_ENCODER_PORT_B 12

#define ENCODER_MAX .000546
#define ENCODER_TO_ANGLE 360.0/ENCODER_MAX
#define ENCODER_SAMPLE_RATE 0.002

//Conversion stuff
#define DRIVE_REDUCTION_HIGH (7.67/3.125) //3.125 is sketchyfix
#define DRIVE_REDUCTION_LOW (30.67/3.125) 
#define TICKS_PER_REVOLUTION 250.0
#define WHEEL_CIRCUMFERENCE 1.571 //in Feet
#define REVOLUTIONS_PER_FOOT (1.0/WHEEL_CIRCUMFERENCE) //At the wheel rev/feet
#define FEET_PER_TICK_HIGH (1.0/(TICKS_PER_REVOLUTION * DRIVE_REDUCTION_HIGH * REVOLUTIONS_PER_FOOT))
#define FEET_PER_TICK_LOW (1.0/(TICKS_PER_REVOLUTION * DRIVE_REDUCTION_LOW * REVOLUTIONS_PER_FOOT))

//PIDs
#define DRIVE_P 1.05
#define DRIVE_I 0.05//0.1
#define DRIVE_D 0.05

#define DRIVE_P_LOW 0.85
#define DRIVE_I_LOW 0.05
#define DRIVE_D_LOW 0.05

#define TURN_P 1.00
#define TURN_I 0.00
#define TURN_D 0.00

#define TURN_P_LOW 2.3
#define TURN_I_LOW 0.0
#define TURN_D_LOW 0.0

#define LIGHT_SENSOR_CHANNEL_FRONT 2
#define LIGHT_SENSOR_CHANNEL_BACK 1
#define LIGHT_SENSOR_CHANNEL_BRIDGE 3
#define LIGHT_CHANNEL 3 //Last value: 1
//Gyro Channel	
#define GYRO_CHANNEL 1

//Max speeds for the wheels in low and high gears
#define MAXSPEEDHIGH 14.7
#define MAXSPEEDLOW  3.7

#define LINE_STOP_SPEED .5 //The speed at which the robot runs to stop at the key
//DriveBase End

//Shooter Start
#define SHOOTER 1
#define SHOOTER_PID 1

//Conversion stuff
#define SHOOTER_UNITS_PER_TICK ((double)0.234375) //(60.0/256.0) Last bit is janky hack 

#define SHOOTER_VIC_CHANNEL1 8				//Channel for shooter wheel
#define SHOOTER_VIC_CHANNEL2 9
#define SHOOTER_HOOD_CHANNEL 0

#define SHOOTER_WHEEL_ENCODER_A 9		//Channels for the shooter wheel's encoder
#define SHOOTER_WHEEL_ENCODER_B 10

//Accuracy tolerance.  How close	 the speed/position has to get to be accurate
#define SHOOTER_VELOCITY_TOLERANCE_LOW	25.0	//Tolerance of the velocity
#define SHOOTER_VELOCITY_TOLERANCE_HIGH 15.0
//SHooter PIDs
#define SHOOTER_P 1.20
#define SHOOTER_I 0.05
#define SHOOTER_D 0.05

#define SHOOTER_READY_STABLITY 10

//Preset shot speeds
#define SHOT_KEYTOP_SPEED 3250.0 

#define SHOT_FREETHROW_SPEED 2525.0

#define SHOT_OTHER_SPEED 0.0

#define SHOOTER_MAX_SPEED 3500.0


#define BALL_SPEED_TO_RPM(speed) {return speed;}   //TODO Conversion

#define AVERAGE_LENGTH 10.0

//Shooter End

//Tipper Start
#define TIPPER_UP_CHANNEL 4
#define TIPPER_DOWN_CHANNEL 3 
//Tipper End

//Collector Start
#define COLLECTOR_VIC_PICKUP 5
#define COLLECTOR_VIC_LOW 6
#define COLLECTOR_VIC_TOP 7
//Number of sensed ball storage spots
#define COLLECTOR_SLOT_COUNT 3
//Belt Vic Speed
#define COLLECTOR_BELT_SPEED 0.4
#define COLLECTOR_FEED_SPEED 1.0
//Collector Vic Speed
#define COLLECTOR_PICKUP_SPEED 0.5
//Timeout for blind running collectors
#define COLLECTOR_TIMEOUT 100
#define SHOOTER_TIMEOUT 30

//Collector IR Sensors
#define COLLECTOR_IR_LOW_CHANNEL 7
#define COLLECTOR_IR_MID_CHANNEL 6
#define COLLECTOR_IR_TOP_CHANNEL 8
//Collector End

//Controls Begin
#define SHIFT_BUTTON lStick->GetRawButton(1)
#define COLLECT_BUTTON (rStick->GetRawButton(1) || !myEIO->GetDigital(12))
#define SHOOT_BUTTON (!myEIO->GetDigital(14))
#define LIGHT_BUTTON 1//rStick->GetRawButton(10)
#define ARM_BUTTON (myEIO->GetDigital(16))
#define FORWARD_SWITCH (!myEIO->GetDigital(4))
#define REVERSE_SWITCH (!myEIO->GetDigital(6))
#define TIPPER_SWITCH (!myEIO->GetDigital(10))
#define SHORT_SHOT_SWITCH (myEIO->GetDigital(8))
#define RPM_MANUAL_SLIDER
#define KEY_ALIGN_BUTTON (!myEIO->GetDigital(15))
#define AUTO_TARGET_BUTTON false// TODO (!myEIO->GetDigital(11))
#define FULL_AUTO_SWTICH (!myEIO->GetDigital(13))
//Slider stuff
#define PID_SLIDER (myEIO->GetAnalogIn(2))
//LEDs
#define FRONT_LINE_LED(s) (myEIO->SetDigitalOutput(7,!s))
#define BACK_LINE_LED(s) (myEIO->SetDigitalOutput(9,!s))
#define AUTO_RANGE_LED(s) (myEIO->SetDigitalOutput(3,!s))
#define AUTO_YAW_LED(s) (myEIO->SetDigitalOutput(5,!s))
#define RPM_LOCK_LED(s) (myEIO->SetDigitalOutput(1,!s))

#endif
