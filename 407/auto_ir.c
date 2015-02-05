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
        return 1;
    }

    if (right <= (-1)*left + 8.33) {
        return 3;
    } else if (right <= (-6.0/5.0)*left + 12) {
        return 2;
    } else {
        return 1;
    }
}

void ir_debug_pos() {
    while (true) {
        writeDebugStreamLine("[ir_debug_pos] Position: %d", ir_position());
        sleep(1000);
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
    sleep(500);
    drive_straight(-pdrive, 500);

    // Drive to position facing goal.
    int position = ir_position();
    switch (position) {
    case 1:
        gyro_turn(45);
        sleep(500);
        drive_straight(pdrive, 1000);
        sleep(500);
        gyro_turn(-90);
        break;
    case 2:
        drive_straight(pdrive, 500);
        break;
    case 3:
        gyro_turn(-45);
        sleep(500);
        drive_straight(pdrive, 1200);
        sleep(500);
        gyro_turn(90);
        break;
    }
}

/**
 * Drive the robot to the correct position at the center goal for the FTC
 * 2014-2015 Cascade Effect game.
 */
void auto_centergoal() {
    auto_ir_prepos();

    // Execute final approach.
    writeDebugStreamLine("[auto_ir_prepos] Going on final approach...");
    auto_ir_dual_final(pdrive, pturn);
    writeDebugStreamLine("[auto_centergoal] At center goal (I hope)");
    drive_straight(pdrive, 500);
}
