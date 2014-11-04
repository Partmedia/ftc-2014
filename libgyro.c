/**
 * @file
 * Gyroscope drive and utility routines
 */

#include "libdrive.c"

/** Gyroscope state and configuration */
typedef struct {
    int port, offset;
    bool reversed;
    long accumulator, turn_offset;
} gyro_conf;

static gyro_conf _gyro;

/**
 * Negate the given number if the gyro is mounted upside down.
 */
int gyro_sign(int angle) {
    if (_gyro.reversed == false) {
        return angle;
    } else {
        return -angle;
    }
}

/**
 * Internal task to continually increment the gyro accumulator.
 */
task gyro_run() {
    while (true) {
        _gyro.accumulator += gyro_sign(SensorValue[_gyro.port] - _gyro.offset);
        wait1Msec(1000 / 10);
    }
}

/**
 * Clear the turn counter.
 */
void gyro_clear() {
    _gyro.turn_offset = _gyro.accumulator;
}

/**
 * Get the current turn gyro heading.
 */
int gyro_heading() {
    return (_gyro.accumulator - _gyro.turn_offset) / 10;
}

/**
 * Get the current absolute gyro heading.
 */
int gyro_heading_abs() {
    return (_gyro.accumulator / 10);
}

/**
 * Calibrate the gyroscope.
 */
void gyro_calibrate() {
    const int samples = 10;
    int average = 0;

    // Calibrate the gyroscope.
    for (int i = 0; i < samples; i++) {
        average += SensorValue[_gyro.port];
        wait1Msec(200);
    }

    average /= samples;
    _gyro.offset = average;
}

/**
 * Configure, calibrate, and start the given gyroscope.
 */
void gyro_init(int port, bool reversed) {
    _gyro.port = port;
    _gyro.reversed = reversed;

    // Clear accumulator, calibrate gyro, and reset turn counter.
    _gyro.accumulator = 0;
    gyro_calibrate();
    gyro_clear();

    // Start grabbing gyro readings.
    StartTask(gyro_run);
}

/**
 * Turn to the given angle relative to the starting orientation.
 */
void gyro_turn_abs(int angle, int speed) {
    while (abs(gyro_heading_abs() - angle) > 5) {
        // Turn right if target angle is greater than current heading.
        if (angle > gyro_heading_abs()) {
            drive_power(speed, -speed);
        } else {
            drive_power(-speed, speed);
        }

        // Keep the loop running below 10 cycles/second.
        wait1Msec(1000 / 10);
    }

    // Stop motors once turn is complete.
    drive_power(0, 0);
}

/**
 * Turn to the given angle relative to the current heading.
 */
void gyro_turn(int angle, int speed) {
    gyro_turn_abs(gyro_heading_abs() + angle, speed);
}