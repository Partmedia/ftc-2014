#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m_right,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     m_left,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S2_C1_1,    gate,                 tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    grabber_r,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    grabber_l,            tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/drive.c"

void init() {
    drive_init(m_left, m_right);
    servo[gate] = 80;
}

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

        //Grabber
        if (joy1Btn(6)) {
	        servo[grabber_l] = 220; //up
            servo[grabber_r] = 35;
        } else if (joy1Btn(8)) {
            servo[grabber_l] = 80;  //down
            servo[grabber_r] = 175;
        }

        // Gate
        if (joy2Btn(5)) {
            servo[gate] = 200;
        } else if (joy2Btn(7)) {
            servo[gate] = 80;
        }
    }
}
