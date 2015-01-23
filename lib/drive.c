/**
 * @file
 * Basic interface for robot drive and manual control
 */

#include "JoystickDriver.c"

#define DEADZONE 10

/** Addresses to the left and right motors, respectively. */
static int _drive_motor_l = -1, _drive_motor_r = -1;

/** Current robot drive power. */
static int _drive_power = 50;

/**
 * Scale power that is not beyond the given threshold.
 */
static int scale_power(int power, int threshold) {
    if (abs(power) < threshold) {
        return 0;
    } else {
        return power;
    }
}

/**
 * Initialize drive train.
 */
void drive_init(int motor_left, int motor_right) {
    playSound(soundUpwardTones);
    _drive_motor_l = motor_left;
    _drive_motor_r = motor_right;
}

/**
 * Set drive power for left and right motors.
 */
void drive_power(int power_left, int power_right) {
    motor[_drive_motor_l] = power_left;
    motor[_drive_motor_r] = power_right;
}

/**
 * Drive straight ahead at the given power for a certain amount of time.
 *
 * @param power Power setting from -100 to 100, inclusive
 * @param msec Time in milliseconds
 */
void drive_straight(int power, int msec) {
    drive_power(power, power);
    wait1Msec(msec);
    drive_power(0, 0);
}

/**
 * Turn by setting the left and right motors in opposite directions.
 *
 * @param power Turning power. Positive values represent right turns.
 * @param msec Time in milliseconds
 */
void drive_turn(int power, int msec) {
    drive_power(power, -power);
    wait1Msec(msec);
    drive_power(0, 0);
}

/**
 * Handle drive train using a dual-axis tank drive.
 */
void drive_joystick_tank() {
    drive_power(scale_power(joystick.joy1_y1, DEADZONE) * _drive_power / 100,
              scale_power(joystick.joy1_y2, DEADZONE) * _drive_power / 100);
}

/**
 * Handle drive train using a dual independent-axis drive.
 */
void drive_joystick_axes() {
    float forward = scale_power(joystick.joy1_y1, DEADZONE) / 100.0;
    float turn = scale_power(joystick.joy1_x2, DEADZONE) / 100.0;

    float multi_left = forward + turn;
    float multi_right = forward - turn;

    drive_power(multi_left * _drive_power, multi_right * _drive_power);
}

/**
 * Handle joystick buttons to control drive power.
 */
void drive_joystick_power() {
    int power_orig = _drive_power;

    if (joy1Btn(1)) {
        _drive_power = 25;
    } else if (joy1Btn(2)) {
        _drive_power = 50;
    } else if (joy1Btn(3)) {
        _drive_power = 75;
    } else if (joy1Btn(4)) {
        _drive_power = 100;
    }

    // Give audible feedback only if power has changed.
    if (power_orig != _drive_power) {
        playSound(soundBlip);
    }
}

/**
 * Preserve backward-compatibility with original joystick handler.
 */
void drive_handle_joystick() {
    drive_joystick_power();
    drive_joystick_tank();
}
