#pragma config(Hubs,  S1, HTMotor,  HTServo,  none,     none)
#pragma config(Sensor, S2,     ir_left,        sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     ir_right,       sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     sp_gyro,        sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C2_1,    sp_right,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    sp_left,              tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/menu.c"
#include "../libgyro.c"

#include "common.c"
#include "ir.c"

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

void init() {
    init_common();
    gyro_init(sp_gyro, false);
}

static void auto_turn(int target, int speed) {
    servo_error result = gyro_turn_abs(target, speed);

    if (result != SERVO_OK) {
        drive_straight(speed, 500);
        auto_turn(target, speed);
    }
}

/**
 * Turn and drive to parking zone.
 */
void goal_to_park(bool inverse) {
    writeDebugStreamLine("[autonomous] Heading %s parking zone...",
        inverse ? "from" : "to");

    if (inverse == false) {
        auto_turn(-130, 100);
        drive_straight(40, 4000);
    } else {
        drive_straight(-40, 4000);
        auto_turn(0, 100);
    }
}

/**
 * Start from parking zone, fetch another goal, and drop in parking zone.
 */
void get_another() {
    grabber_up(true);
    goal_to_park(true);
    drive_straight(40, 2000);
    grabber_down(true);
    drive_straight(-40, 2000);
    goal_to_park(false);
    grabber_up(false);
}

void start_ground() {
    writeDebugStreamLine("[autonomous] Starting from parking zone...");
    auto_centergoal(ir_left, ir_right);
}

void start_ramp() {
    // Drive off ramp and towards rolling goals.
    writeDebugStreamLine("[autonomous] Starting from ramp...");
    drive_straight(25, 4500);

    // Grab rolling goal and dump ball.
    grabber_down(true);
    drive_straight(-40, 2000);

    // Go to parking zone and release goal.
    goal_to_park(false);
}

task main() {
    writeDebugStreamLine("[autonomous] Started!");
    init_config();
    waitForStart();
    init();

    if (c_delay.value != 0) {
        writeDebugStreamLine("[autonomous] Waiting for %d seconds...", c_delay.value);
        Sleep(c_delay.value * 1000);
    }

    if (c_ramp.value) {
        start_ramp();
    } else {
        start_ground();
    }

    writeDebugStreamLine("[autonomous] Finished!");
}
