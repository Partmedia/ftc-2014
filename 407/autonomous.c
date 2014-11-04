#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     motor_right,   tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motor_left,    tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motor_carriage, tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     motorG,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    grabber_left,         tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber_right,        tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libdrive.c"
#include "../libgyro.c"

/**
 * Move the grabber to the up position.
 * @todo Put actual values here.
 */
void grabber_up() {
    servo[grabber_left] = 50;
    servo[grabber_right] = 50;
}

/**
 * Move the grabber to the down position.
 * @todo Put actual values here.
 */
void grabber_down() {
    servo[grabber_left] = 50;
    servo[grabber_right] = 50;
}

void init() {
    drive_init(motor_left, motor_right);
    gyro_init(gyro, false);
    grabber_up();
    gyro_calibrate();
}

task main() {
    waitForStart();
    init();

    // Drive off ramp and towards rolling goals.
    drive_straight(50, 1000);
    drive_straight(60, 1500);

    // Grab rolling goal and dump ball.
    grabber_down();

    // Turn and drive towards parking zone.
    gyro_turn_abs(-135, 70);
    drive_straight(50, 1000);

    // Release rolling goal.
    grabber_up();
}