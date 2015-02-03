/**
 * Return a random number between min and max, inclusive.
 */
int rand_int(int min, int max) {
    return random[max - min] + min;
}

/**
 * Return false if a potential battery problem is detected.
 */
bool check_battery() {
    writeDebugStreamLine("[check_battery] Ext: %.2f, NXT: %.2f",
            externalBatteryAvg / 1000.0, nAvgBatteryLevel / 1000.0);

    if (externalBatteryAvg < 12 * 1000) {
        writeDebugStreamLine("[check_battery] WARNING: External battery LOW");
        return false;
    } else if (nAvgBatteryLevel < 6 * 1000) {
        writeDebugStreamLine("[check_battery] WARNING: NXT battery LOW");
        return false;
    } else {
        return true;
    }
}

/**
 * Check for various problems and complain until they are fixed.
 */
task check() {
    while (true) {
        if (check_battery() == false) {
            playSound(soundException);
        }

        wait1Msec(5 * 1000);
    }
}

/**
 * Start background sanity checking.
 */
void check_init() {
    startTask(check);
}
