/**
 * @file
 * Autonomous robot actions and behaviors
 */

#include "gyro.c"

static int _ir_port = -1;

/**
 * Initialize the IR sensor.
 */
void ir_init(int port) {
    _ir_port = port;
}

/**
 * Accurately aim robot at the IR beacon.
 */
int ir_init_turn(int ir, int speed) {
    // Get out of the cone if we are facing beacon.
    while (SensorValue[ir] == 5) {
        playSound(soundBlip);
        drive_power(-speed, speed);
    }

    bool turn_right;

    // Figure out which way to turn next.
    if (SensorValue[ir] > 5) {
        turn_right = true;
    } else {
        turn_right = false;
    }

    bool had_gyro = false;
    int bound_end;

    // Get back in range.
    while (true) {
        if (turn_right) {
            drive_power(speed, -speed);
            playSound(soundUpwardTones);
        } else {
            drive_power(-speed, speed);
            playSound(soundDownwardTones);
        }

        if (had_gyro == false) {
            // If we see a '5', clear the turn accumulator.
            if (SensorValue[ir] == 5) {
                playSound(soundLowBuzz);
                had_gyro = true;
                gyro_clear();
            }
        } else {
            // If we don't see a '5', record the heading and break out.
            if (SensorValue[ir] != 5) {
                bound_end = gyro_heading();
                drive_power(0, 0);
                break;
            }
        }
    }

    // Turn to the angle in between the two cones.
    int turn_angle = bound_end / 2;

    clearSounds();
    playSound(soundBeepBeep);

    gyro_turn(turn_angle, speed);

    playSound(soundBeepBeep);
    wait1Msec(3000);

    // Return 1 if IR is on the right, -1 if on the left.
    if (-turn_angle > 0) {
        return 1;
    } else {
        return -1;
    }
}
