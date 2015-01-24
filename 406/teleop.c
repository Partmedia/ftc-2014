#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          Stopper,       tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     Rack,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     Lever,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     Right,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     Left,          tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     Arm,           tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/drive.c"

void init() {
    drive_init(Left, Right);
}

task main() {
    waitForStart();
	init();

	while (true) {
	    getJoystickSettings(joystick);
        drive_handle_joystick();
        motor[Rack] = joystick.joy2_y1 * 0.5;
        motor[Lever] = joystick.joy2_y2 * 0.25;

        if (joy2Btn(6)) {
            motor[Arm] = 30;
        } else if (joy2Btn(8)) {
            motor[Arm] = -20;
        } else {
            motor[Arm] = 0;
        }

        // Servo controls (moveable goal grabbers). //
        if (joy1Btn(5)) {
            servo[servo1] = 45;
        }
        if (joy1Btn(7)) {
            servo[servo1] = -45;
        }
        if (joy1Btn(6)) {
            servo[servo2] = 45;
        }
        if (joy1Btn(8)) {
            servo[servo2] = -45;
        }

        // Moving both wheels at once. //
        if (joystick.joy1_TopHat == 0) {
            motor[Left] = 65;
            motor[Right] = 70;
        } else if (joystick.joy1_TopHat == 4) {
            motor[Left] = -65;
            motor[Right] = -70;
        } else {
            motor[Left] = 0;
            motor[Right] = 0;
        }
    }
}
