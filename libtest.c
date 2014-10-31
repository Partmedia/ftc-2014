/**
 * @file
 * Status checks and unit testing for libraries
 */

#include "libgyro.c"

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
    if (externalBatteryAvg < 12 * 1000 || nAvgBatteryLevel < 6 * 1000) {
        return false;
    }

    return true;
}

/**
 * Check for various problems and complain until they are fixed.
 */
task check() {
    while (true) {
        if (check_battery() == false) {
            PlaySound(soundException);
        }

        wait1Msec(5 * 1000);
    }
}

/**
 * Start background sanity checking.
 */
void check_init() {
    StartTask(check);
}

bool test_gyro() {
    for (int i = 0; i < 20; i++) {
        int angle = 90 * rand_int(1, 6) * sgn(rand_int(0, 10) - 5);
        gyro_turn(angle, rand_int(40, 60));

        writeDebugStream("Case %d: %d out of %d\n", i + 1, gyro_heading(), angle);
        PlaySound(soundBeepBeep);
    }

    writeDebugStream("===>>> Passed gyro test!");
    PlaySound(soundUpwardTones);
    return true;
}
