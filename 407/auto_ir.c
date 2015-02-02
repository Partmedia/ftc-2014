#include "../lib/drive.c"
#include "../lib/gyro.c"
#include "../lib/ir.c"

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

int ir_position() {
    ir_update();
    if (ir_left.acDirection == 3) {
        return 3;
    } else if (ir_left.acDirection == 5) {
        return 2;
    } else if (ir_left.acDirection == 0) {
        return 1;
    } else {
        writeDebugStreamLine("[ir_position] Unknown position!");
    }
}

/**
 * Pre-position the robot so that it is ready for final approach towards the
 * 120 cm goal in the FTC 2014-2015 Cascade Effect game.
 */
void auto_ir_prepos() {
    drive_straight(pdrive, 500);
    gyro_turn_abs(-45);
    drive_straight(pdrive, 1000);
    gyro_turn_abs(45);
    drive_straight(pdrive, 300);

    ir_debug();

    while (true) {
        writeDebugStreamLine("[auto_ir_prepos] Position: %d", ir_position());
        sleep(1000);
    }

    if (ir_dual_ready_final()) {
        writeDebugStreamLine("[auto_ir_prepos] Straight ahead!");
        auto_ir_dual_final(pdrive, pturn);
        return;
    } else {
         writeDebugStreamLine("[auto_ir_prepos] Not straight ahead...");
    }

    // Move forwards until we detect IR beacon perpendicularly.
    //while (SensorValue[ir_right] <= 7) {
    //    drive_straight(pdrive, 250);
    //}

    // Check if IR beacon is in second position.
    gyro_turn_abs(45);
    if (ir_dual_ready_final()) {
        writeDebugStreamLine("[auto_ir_prepos] Position 2!");
        auto_ir_dual_final(pdrive, pturn);
        return;
    } else {
        writeDebugStreamLine("[auto_ir_prepos] Not in position 2...");
    }

    // Move to third center goal position.
    gyro_turn_abs(0);
    drive_straight(pdrive, 1000);
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
