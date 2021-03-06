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
#include "auto_ir.c"

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
    ir_dual_init(irp_left, irp_right);
    gyro_init(sp_gyro, false);
}

/**
 * Autonomously turn to a given target, correcting if necessary.
 */
static void auto_turn(int target, int speed) {
    gyro_error result = gyro_turn_abs(target);
    if (result != GYRO_OK) {
        drive_straight(speed, 500);
        auto_turn(target, speed);
    }
}

void start_ground() {
    writeDebugStreamLine("[autonomous] Starting from parking zone...");
    auto_centergoal();

    // Raise rack
    motor[m_rack] = 70;
    sleep(6500);
    motor[m_rack] = 0;

    // Deploy arm
    motor[m_arm] = 35;
    sleep(2000);
    motor[m_arm] = 0;

    // Drop ball
    sleep(1000);
    servo_open(&claw);
}

void start_ramp() {
    // Drive off ramp and towards rolling goals.
    writeDebugStreamLine("[autonomous] Starting from ramp...");
    drive_straight(25, 3500);

    // Grab rolling goal and back away.
    grabber_down(true);
    drive_straight(-40, 2000);

    // Go to parking zone and release goal.
    writeDebugStreamLine("[autonomous] Heading to parking zone...");
    auto_turn(-90, 100);
    drive_straight(50, 1000);
    auto_turn(-160, 100);
    drive_straight(50, 3000);
}

task main() {
    writeDebugStreamLine("[autonomous] Started!");
    check_battery();
    power_adjust();
    init_config();
    displayTextLine(0, "<407> Autonomous");
    waitForStart();
    init();

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
