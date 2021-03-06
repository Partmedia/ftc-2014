#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  none)
#pragma config(Sensor, S2,     irp_left,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     irp_right,      sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     sp_gyro,        sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_1,     m_arm,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     m_rack,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    sp_right,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    sp_left,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    sp_claw,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    sp_scoop,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "common.c"

static void init() {
    _drive_power = 100;
}

task main() {
    writeDebugStreamLine("[teleop] Started!");
    waitForStart();
    init_common();
    init();

    while (true) {
        getJoystickSettings(joystick);
        drive_joystick_power();
        drive_joystick_axes();

        if (joy1Btn(8)) {
            grabber_down(false);
        } else if (joy1Btn(6)) {
            grabber_up(false);
        }

        // Ball scoop (controller 1)
        if (joy1Btn(5)) {
            servo_open(&scoop);
        } else {
            servo_close(&scoop);
        }

        // Rack (controller 2)
        motor[m_rack] = joystick.joy2_y1;

        // Ball arm (controller 2)
        motor[m_arm] = joystick.joy2_y2 * 0.35;

        // Ball-grabbing claw (controller 2)
        if (joy2Btn(8)) {
            servo[sp_claw] = 165;
        } else if (joy2Btn(6)) {
            servo[sp_claw] = 135;
        }

        // Limit loop to 10 updates per second.
        sleep(1000 / 10);
    }
}
