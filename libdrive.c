/**
 * libdrive.c -- drive the robot
 */

#define _LIBDRIVE_C

#include "JoystickDriver.c"

#define DEADZONE 10

/** Addresses to the left and right motors, respectively. */
static int _drive_motor_l = -1, _drive_motor_r = -1;

/** Current robot drive power. */
static int _drive_power = 50;

/** Play audible sounds if set to true. */
static bool _drive_debug = true;

/**
 * Scale power that is not beyond the given threshold.
 */
int power_scale(int power, int threshold) {
    if (abs(power) < threshold) {
        return 0;
    } else {
        return power;
    }
}

void set_drive_debug(bool debug) {
    _drive_debug = debug;
}

/**
 * Initialize drive train by setting left and right motors.
 */
void set_drivetrain(int motor_left, int motor_right) {
    if (_drive_debug) {
        PlaySound(soundUpwardTones);
    }

    _drive_motor_l = motor_left;
    _drive_motor_r = motor_right;
}

/**
 * Set power to the left and right motors, respectively.
 */
void set_power(int power_left, int power_right) {
    motor[_drive_motor_l] = power_left;
    motor[_drive_motor_r] = power_right;
}

/**
 * Drive straight using the given power for a certain time.
 */
void drive(int power, int msec) {
    set_power(power, power);
    wait1Msec(msec);
    set_power(0, 0);
}

/**
 * Turn using the given power for a certain amount of time.
 */
void drive_turn_time(int power, int msec) {
    set_power(power, -power);
    wait1Msec(msec);
    set_power(0, 0);
}

/**
 * Set the drive power based on pressed joystick buttons.
 */
void joystick_set_power() {
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

    // If the power has changed, play debug sound.
    if (power_orig != _drive_power) {
        if (_drive_debug) {
            PlaySound(soundBlip);
        }
    }
}

/**
 * Drive the robot based on joystick settings.
 */
void joystick_drive() {
    set_power(power_scale(joystick.joy1_y1, DEADZONE) * _drive_power / 100,
              power_scale(joystick.joy1_y2, DEADZONE) * _drive_power / 100);
}
