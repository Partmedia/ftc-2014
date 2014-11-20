#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Rack,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lever,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Right,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     Left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libdrive.c"

void init() {
    drive_init(Left, Right);
}

task main() {
    waitForStart();
	init();

	while (true) {
        drive_handle_joystick();
    }
}
