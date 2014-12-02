#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     rack_stop_up,   sensorTouch)
#pragma config(Sensor, S4,     rack_stop_down, sensorTouch)
#pragma config(Motor,  mtr_S1_C2_1,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     m_rack,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     m_conveyor,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    sgl_port,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    sgr_port,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "common.c"

typedef enum {
    DRIVE_FORWARD, DRIVE_BACKWARD
} drivedir;

// Currently, init_common() starts robot in backwards direction.
static drivedir current_dir = DRIVE_BACKWARD;

static void drive_dir_set(drivedir dir) {
    // Avoid setting the same direction more than once.
    if (dir == current_dir) {
        return;
    } else {
        current_dir = dir;
    }

    if (dir == DRIVE_FORWARD) {
        drive_init(m_right, m_left);
    } else if (dir == DRIVE_BACKWARD) {
        drive_init(m_left, m_right);
    }

    // @todo Replace this hack:
    _drive_power *= -1;
}

static void drive_switch_handle() {
    if (joystick.joy1_TopHat == 0) {
        drive_dir_set(DRIVE_FORWARD);
    } else if (joystick.joy1_TopHat == 4) {
        drive_dir_set(DRIVE_BACKWARD);
    }
}

static void handle_conveyor() {
    if (joy2Btn(1)) {
        motor[m_conveyor] = -15;
    } else if (joy2Btn(2)) {
        motor[m_conveyor] = 50;
    } else if (joy2Btn(3)) {
        motor[m_conveyor] = 31;
    } else if (joy2Btn(4)) {
        motor[m_conveyor] = 25;
    } else {
        motor[m_conveyor] = 0;
    }
}

static void handle_rack() {
    // Rack controls
    if (joy2Btn(5)) {
        motor[m_rack] = 60;
    } else if (joy2Btn(7)) {
        motor[m_rack] = -50;
    } else {
        // Manually control rack while buttons are not pressed.
        motor[m_rack] = joystick.joy2_y1;
    }

    // Prevent rack from going up when upper limit is reached.
    if (SensorValue[rack_stop_up] == true) {
        if (motor[m_rack] > 0) {
            motor[m_rack] = 0;
        }
    } else if (SensorValue[rack_stop_down] == true) {
        if (motor[m_rack] < 0) {
            motor[m_rack] = 0;
        }
    }
}

static void init() {
    // No-op
}

task main() {
    waitForStart();
    init_common();
    init();

    while (true) {
        drive_handle_joystick();
        drive_switch_handle();

        if (joy1Btn(6)) {
            grabber_up();
        } else if (joy1Btn(8)) {
            grabber_down();
        }

        handle_rack();
        handle_conveyor();
    }
}
