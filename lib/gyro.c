/**
 * @file
 * Gyroscope drive and utility routines
 */

#define GYRO_TIMER T1

int gyro_timeout = 5000;    //< Gyro turn timeout in milliseconds
int gyro_tolerance = 3;     //< Tolerance for gyro target in degrees

enum gyro_error {
    GYRO_OK,
    GYRO_CAL_ERROR,         //< Gyro calibration error
    GYRO_TIMEOUT,           //< Timed out while turning to target
};

typedef struct {
    int port;               //< Port number given by RobotC
    int offset;             //< Gyro calibration offset
    bool reversed;          //< True if the gyro is mounted upside down
    long accumulator;       //< Current gyro heading, 10 per degree
} gyro_conf;

gyro_conf gyro;             //< Global gyroscope state

/**
 * Negate the given number if the gyro is mounted upside down.
 */
static int gyro_sign(int angle) {
    if (gyro.reversed == false) {
        return angle;
    } else {
        return -angle;
    }
}

/**
 * Internal task to continually increment the gyro accumulator.
 */
task gyro_run() {
    long last_update = 0;
    while (true) {
        long current_time = nPgmTime;
        if (current_time >= last_update + 100) {
            gyro.accumulator += gyro_sign(SensorValue[gyro.port] - gyro.offset);
            last_update = current_time;
        }
        sleep(10);
    }
}

/**
 * Get the current absolute gyro heading, wrapped around at 360 degrees.
 */
int gyro_heading_abs() {
    return (gyro.accumulator / 10) % 360;
}

/**
 * Calibrate the gyroscope.
 */
gyro_error gyro_calibrate() {
    const int samples = 10;
    int average = 0;

    // Calibrate the gyroscope.
    for (int i = 0; i < samples; i++) {
        average += SensorValue[gyro.port];
        sleep(100);
    }

    average /= samples;
    writeDebugStreamLine("[gyro] Calibration: %d after %d samples",
            average, samples);

    // Check for totally ridiculous calibration results.
    if (abs(average - 620) > 300) {
        writeDebugStreamLine("[gyro] Calibration FAILED; disabling gyro");
        return GYRO_CAL_ERROR;
    } else {
        writeDebugStreamLine("[gyro] Calibration OK");
        gyro.offset = average;
        return GYRO_OK;
    }
}

/**
 * Configure, calibrate, and start the given gyroscope.
 */
void gyro_init(int port, bool reversed) {
    gyro.port = port;
    gyro.reversed = reversed;

    // Clear accumulator and calibrate gyro.
    gyro.accumulator = 0;
    gyro_calibrate();

    // Start grabbing gyro readings.
    startTask(gyro_run);
}

/**
 * Calculate target turn rate based on error.
 */
int gyro_target_rate(int error) {
    if (error < 30) {
        return 30;
    } else {
        return 80;
    }
}

/**
 * Turn to the given angle relative to the starting orientation.
 * @param target Target heading relative to initial heading
 */
gyro_error gyro_turn_abs(int target) {
    const int k = 2;                // Speed adjustment per cycle
    const int l_wait = 1000 / 5;    // Loop at about 5 cycles per second
    int speed = 100;                // Initial speed
    clearTimer(GYRO_TIMER);

    while (true) {
        int error = abs(gyro_heading_abs() - target);
        if (error < gyro_tolerance) {
            break;
        }

        // Turn right if target angle is greater than current heading.
        if (target > gyro_heading_abs()) {
            drive_power(speed, -speed);
        } else {
            drive_power(-speed, speed);
        }

        // Break out of the loop in case of a timeout.
        if (time1[GYRO_TIMER] > gyro_timeout) {
            writeDebugStreamLine("[gyro] Timeout at %d for target %d",
                    gyro_heading_abs(), target);
            drive_power(0, 0);
            return GYRO_TIMEOUT;
        }

        // Calculate turn rate.
        int obs_last = gyro_heading_abs();
        sleep(l_wait);
        int obs_rate = abs((gyro_heading_abs() - obs_last) / (l_wait / 1000.0));

        // Adjust speed according to turn rate.
        if (obs_rate > gyro_target_rate(error)) {
            speed -= k;
        } else {
            speed += k;
        }

        writeDebugStreamLine("[gyro_turn_abs] Rate: %d, Speed: %d",
                obs_rate, speed);
    }

    // Stop motors once turn is complete.
    drive_power(0, 0);
    return GYRO_OK;
}

/**
 * Turn to the given angle relative to the current heading.
 */
gyro_error gyro_turn(int target) {
    return gyro_turn_abs(gyro_heading_abs() + target);
}
