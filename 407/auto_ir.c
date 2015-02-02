#include "../lib/lib.h"

const int pdrive = 40, pturn = 70;

/**
 * Initialize the robot and drop into an infinite loop to debug IR.
 */
void ir_debug() {
    while (true) {
        ir_update();
        writeDebugStreamLine("[ir_debug] %d, %d",
                ir_left.acDirection, ir_right.acDirection);
        sleep(1000);
    }
}

/**
 * Determine the position of the center goal.
 */
int ir_position() {
    // Average a bunch of IR readings.
    const int count = 5;
    int left = 0, right = 0;
    for (int i = 0; i < count; i++) {
        ir_update();
        left += ir_left.acDirection;
        right += ir_right.acDirection;
        sleep(1000 / count);
    }
    left /= count;
    right /= count;
    writeDebugStreamLine("[ir_position] Left %d, Right %d", left, right);

    // Make exceptions when sensors return zero.
    if (left == 0) {
        return 2;
    } else if (right == 0) {
        return 1;
    }

    if (right <= -5.0/6.0*left + 6.72) {
        return 3;
    } else if (right <= -3*left + 24) {
        return 2;
    } else {
        return 1;
    }
}

/**
 * Pre-position the robot so that it is ready for final approach towards the
 * 120 cm goal in the FTC 2014-2015 Cascade Effect game.
 */
void auto_ir_prepos() {
    drive_straight(pdrive, 1750);
    sleep(500);
    gyro_turn_abs(90);

    while (true) {
        writeDebugStreamLine("[auto_ir_prepos] Position: %d", ir_position());
        sleep(1000);
    }

    // TODO: Implement part that drives to final position.
    writeDebugStreamLine("[auto_ir_prepos] Going on final approach...");
    auto_ir_dual_final(pdrive, pturn);
}

/**
 * Drive the robot to the correct position at the center goal for the FTC
 * 2014-2015 Cascade Effect game.
 */
void auto_centergoal() {
    auto_ir_prepos();
    auto_ir_dual_final(pdrive, pturn);
    writeDebugStreamLine("[auto_centergoal] At center goal (I hope)");
}
