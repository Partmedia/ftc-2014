const int pdrive = 80, pturn = 100;

/**
 * Return a value given by reading sensor values from two IR seekers.
 */
int ir_dual_diffsum(int ir_left, int ir_right) {
    int ldiff = SensorValue[ir_left] - 5;
    int rdiff = SensorValue[ir_right] - 5;
    return ldiff + rdiff;
}

/**
 * Determine which position the center goal is facing in the FTC 2014-2015
 * Cascade Effect game.
 */
int auto_ir_init_pos(int ir_left, int ir_right) {
}

/**
 * Pre-position the robot so that it is ready for final approach towards the
 * 120 cm goal in the FTC 2014-2015 Cascade Effect game.
 */
void auto_ir_prepos(int ir_left, int ir_right) {
    const int diffsum = ir_dual_diffsum(ir_left, ir_right);
    writeDebugStreamLine("[auto_ir_prepos] Diffsum %d", diffsum);
}

/**
 * Execute an autonomous final approach using two IR seekers. The robot will
 * stop right in front of an IR beacon centered between the two sensors.
 */
void auto_ir_dual_final(int ir_left, int ir_right) {
    const int tconst = 200;     //< Time to execute each correction movement
    const int turnthresh = 1;   //< Difference to begin turn

    while (!(SensorValue[ir_left] == 7 && SensorValue[ir_right] == 3)) {
        // If we lose the signal break out of the loop.
        if (SensorValue[ir_left] == 0 || SensorValue[ir_right] == 0) {
            writeDebugStreamLine("[auto_ir_dual_final] Signal lost; break");
            break;
        }

        const int diffsum = ir_dual_diffsum(ir_left, ir_right);
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
 * Drive the robot to the correct position at the center goal for the FTC
 * 2014-2015 Cascade Effect game.
 */
void auto_centergoal(int ir_left, int ir_right) {
    auto_ir_prepos(ir_left, ir_right);
    auto_ir_dual_final(ir_left, ir_right);
    writeDebugStreamLine("[auto_centergoal] At center goal (I hope)");
}
