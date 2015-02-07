#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    gate,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    sp_grabber_l,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    sp_grabber_r,         tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/lib.h"
#include "common.c"

task main() {
    waitForStart();
    init();

    while (true) {
        getJoystickSettings(joystick);
        drive_handle_joystick();

        // Lift
        if (joy2Btn(6)) {
            motor[lift] = 100;  //lift up
        } else if (joy2Btn(8)) {
            motor[lift] = -20;  //lift down
        } else {
            motor[lift] = 0;
        }

        if (joy1Btn(6)) {
	        grabber_up();
        } else if (joy1Btn(8)) {
            grabber_down();
        }

        if (joy2Btn(1)) {
            grabber_up();
        } else if (joy2Btn(2)) {
            grabber_down();
        }

        if (joy2Btn(5)) {
            servo_close(&servo_gate);
        } else if (joy2Btn(7)) {
            servo_open(&servo_gate);
        }
    }
}
