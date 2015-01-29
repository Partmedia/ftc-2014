#include "sensors/hitechnic-irseeker-v2.h"

#include "drive.c"
#include "gyro.c"

tHTIRS2 ir_left, ir_right;
static bool ir_dual;

/**
 * Initialize a single center-mounted IR sensor.
 */
void ir_init(tSensors ir_port) {
    initSensor(&ir_left, ir_port);
    ir_dual = false;
}

/**
 * Initialize two side-mounted IR sensors.
 */
void ir_dual_init(tSensors ir_port_left, tSensors ir_port_right) {
    initSensor(&ir_left, S2);
    initSensor(&ir_right, S3);
    ir_dual = true;
}

/**
 * Update IR sensor values.
 */
void ir_update() {
    readSensor(&ir_left);

    if (ir_dual) {
        readSensor(&ir_right);
    }
}

/**
 * Return a value given by reading sensor values from two IR seekers.
 */
int ir_dual_diffsum() {
    ASSERT(ir_dual);
    ir_update();
    int ldiff = ir_left.acDirection - 5;
    int rdiff = ir_right.acDirection - 5;
    return ldiff + rdiff;
}

/**
 * Check if a robot with dual IR sensors is lined up in front of a beacon.
 */
bool ir_dual_ready_final() {
    ASSERT(ir_dual);
    if (abs(ir_dual_diffsum()) < 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * Execute an autonomous final approach using two IR seekers. The robot will
 * stop right in front of an IR beacon centered between the two sensors.
 */
void auto_ir_dual_final(int pdrive, int pturn) {
    ASSERT(ir_dual);
    const int tconst = 200;     //< Time to execute each correction movement
    const int turnthresh = 1;   //< Difference to begin turn

    while (!(SensorValue[ir_left] == 7 && SensorValue[ir_right] == 3)) {
        // If we lose the signal break out of the loop.
        if (SensorValue[ir_left] == 0 || SensorValue[ir_right] == 0) {
            writeDebugStreamLine("[auto_ir_dual_final] Signal lost; break");
            break;
        }

        const int diffsum = ir_dual_diffsum();
        writeDebugStreamLine("[auto_ir_dual_final] Diffsum %d L:%d, R:%d",
                diffsum, SensorValue[ir_left], SensorValue[ir_right]);
        if (diffsum > turnthresh) {
            drive_turn(pturn, tconst);
        } else if (diffsum < -turnthresh) {
            drive_turn(-pturn, tconst);
        } else {
            drive_straight(pdrive, tconst);
        }
    }
}

/**
 * Accurately aim robot at the IR beacon.
 */
int ir_turn_midpoint(int speed) {
    // In a single-sensor setup, ir_left is the only IR sensor.
    tHTIRS2 *ir = &ir_left;

    // Get out of the cone if we are facing beacon.
    while (ir->acDirection == 5) {
        playSound(soundBlip);
        drive_power(-speed, speed);
    }

    bool turn_right;

    // Figure out which way to turn next.
    if (ir->acDirection > 5) {
        turn_right = true;
    } else {
        turn_right = false;
    }

    bool had_gyro = false;
    int bound_lower, bound_upper;

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
            // If we see a '5', set lower bound.
            if (ir->acDirection == 5) {
                playSound(soundLowBuzz);
                had_gyro = true;
                bound_lower = gyro_heading_abs();
            }
        } else {
            // If we don't see a '5', record the heading and break out.
            if (ir->acDirection != 5) {
                bound_upper = gyro_heading_abs();
                drive_power(0, 0);
                break;
            }
        }
    }

    // Turn to the angle in between the two cones.
    int turn_angle = (bound_upper - bound_lower) / 2;

    clearSounds();
    playSound(soundBeepBeep);

    gyro_turn(turn_angle);

    playSound(soundBeepBeep);
    wait1Msec(3000);

    // Return 1 if IR is on the right, -1 if on the left.
    if (-turn_angle > 0) {
        return 1;
    } else {
        return -1;
    }
}
