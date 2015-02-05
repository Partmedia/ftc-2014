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

config_opt c_ramp;
config_opt c_delay;

void init_config() {
    c_ramp.name = "Ramp";
    c_ramp.value = 0;
    c_ramp.levels[0] = 0;
    c_ramp.levels[1] = 1;
    c_ramp.levels[2] = CONFIG_END;

    c_delay.name = "Delay";
    c_delay.value = 0;
    c_delay.levels[0] = 0;
    c_delay.levels[1] = 5;
    c_delay.levels[2] = 10;
    c_delay.levels[3] = 15;
    c_delay.levels[4] = CONFIG_END;

    config_opt_list list;
    list.opts[0] = &c_ramp;
    list.opts[1] = &c_delay;
    list.length = 2;
    config_menu(list);
    writeDebugStreamLine("[init_config] Configuration OK");
}

void grabber_up() {
    servo[sp_grabber_l] = 220;
    servo[sp_grabber_r] = 35;
}

void grabber_down() {
    servo[sp_grabber_l] = 80;
    servo[sp_grabber_r] = 175;
}

void lift_up() {
    motor[lift] = 100;
    sleep(4100);
    motor[lift] = 0;
}

void lift_down() {
    motor[lift] = -20;
    sleep(3000);
    motor[lift] = 0;
}

void turn_180() {
    motor[m_right] = 30;
    motor[m_left] = -30;
    wait1Msec(2000);
    motor[m_right] = 0;
    motor[m_left] = 0;

}

void turn_135() {
    motor[m_right] = -30;
    motor[m_left] = 30;
    wait1Msec(1500);
    motor[m_right] = 0;
    motor[m_left] = 0;
}

void init() {
    servo[gate] = 200;
    grabber_up();
    drive_init(m_left, m_right);
}

void start_ground() {
    writeDebugStreamLine("[autonomous] Starting from parking zone...");
    drive_straight(-30, 3000);  // move backwards @30% power for 3s
    grabber_down();
    lift_up();
    servo[gate] = 80; //open gate
    sleep(1000);	//wait for ball to roll into goal
    servo[gate] = 200;	//close gate
    lift_down();
    wait1Msec(1000);
    turn_180();
    drive_straight(-30, 3000);  // move backwards @30% power for 3s
}

void start_ramp() {
    drive_straight(-30, 3000);  // move backwards @30% power for 3s
    grabber_down();
    lift_up();
    servo[gate] = 80; //open gate
    sleep(1000);	//wait for ball to roll into goal
    servo[gate] = 200;	//close gate
    lift_down();
    wait1Msec(1000);
    turn_135();
    drive_straight(-30, 3000);  // move backwards @30% power for 3s
}

task main() {
    bool on_ramp = true;
    init_config();
    init();
    waitForStart();

    if (c_delay.value != 0) {
        writeDebugStreamLine("[autonomous] Waiting for %d seconds...", c_delay.value);
        sleep(c_delay.value * 1000);
    }

    if (c_ramp.value) {
        start_ramp();
    } else {
        start_ground();
    }

    writeDebugStreamLine("[autonomous] Finished!");

}
