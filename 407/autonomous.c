#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  none)
#pragma config(Sensor, S2,     gyro,           sensorI2CHiTechnicGyro)
#pragma config(Sensor, S3,     rack_stop_up,   sensorTouch)
#pragma config(Sensor, S4,     rack_stop_down, sensorTouch)
#pragma config(Motor,  mtr_S1_C2_1,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     m_rack,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     m_conveyor,    tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    sgl_port,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    sgr_port,             tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../libgyro.c"

#include "common.c"

typedef enum {
    CONFIG_RAMP, CONFIG_DELAY, CONFIG_SIZE
} option_keys;

const string config_names[CONFIG_SIZE] = {
    "Ramp",
    "Delay",
};

const int config_levels[CONFIG_SIZE][] = {
    {0, 1, -1, 0, 0},
    {0, 5, 10, 15, -1},
};

int config[CONFIG_SIZE];

int config_get(option_keys option) {
    return config_levels[option][config[option]];
}

/**
 * Count the number of configuration levels for each given option.
 */
static int config_count(int option) {
    for (int i = 0; true; i++) {
        if (config_levels[option][i] == -1) {
            return i;
        }
    }
}

/**
 * Display menu with autonomous program configuration options.
 */
static void config_menu() {
    // Manually handle button actions.
    nNxtButtonTask = -2;
    int state = 0, selection = 0;

    while (true) {
        int numbopts = config_count(state);
        int button;

        // Redraw configuration menu.
        eraseDisplay();
        nxtDisplayTextLine(0, "%s < %d >", config_names[state], config_levels[state][selection]);

        // Wait for a button press.
        while (true) {
            button = nNxtButtonPressed;
            if (button != kNoButton) {
                break;
            }
            Sleep(100);
        }

        // Process button press.
        switch (button) {
            case kLeftButton:
                if (selection - 1 >= 0) {
                    selection -= 1;
                }
                break;
            case kRightButton:
                if (selection + 1 < numbopts) {
                    selection += 1;
                }
                break;
            case kEnterButton:
                config[state] = selection;
                selection = 0;
                state++;
                if (state == (short)CONFIG_SIZE) {
                    eraseDisplay();
                    return;
                }
                break;
            default:
                break;
        }

        Sleep(250);
    }
}

void init() {
    init_common();
    gyro_init(gyro, false);
    gyro_calibrate();
}

/**
 * Turn and drive to parking zone.
 */
void goal_to_park(bool inverse) {
    writeDebugStreamLine("[autonomous] Heading %s parking zone...",
        inverse ? "from" : "to");

    if (inverse == false) {
        gyro_turn_abs(-135, 100);
        drive_straight(40, 4000);
    } else {
        drive_straight(-40, 4000);
        gyro_turn_abs(0, 100);
    }
}

/**
 * Start from parking zone, fetch another goal, and drop in parking zone.
 */
void get_another() {
    grabber_up(true);
    goal_to_park(true);
    drive_straight(40, 2000);
    grabber_down(true);
    drive_straight(-40, 2000);
    goal_to_park(false);
    grabber_up(false);
}

void start_ground() {
    writeDebugStreamLine("[autonomous] Starting from parking zone...");
    drive_straight(40, 500);
    gyro_turn_abs(-135, 100);
    get_another();
    get_another();
}

void start_ramp() {
    // Drive off ramp and towards rolling goals.
    writeDebugStreamLine("[autonomous] Starting from ramp...");
    drive_straight(25, 4500);

    // Grab rolling goal and dump ball.
    grabber_down(true);
    drive_straight(-40, 2000);
#if 0
    motor[m_conveyor] = 40;
    Sleep(2000);
    motor[m_conveyor] = 0;
#endif

    // Go to parking zone and release goal.
    goal_to_park(false);
}

task main() {
    writeDebugStreamLine("[autonomous] Started!");
    config_menu();

    waitForStart();
    init();

    Sleep(config_get(CONFIG_DELAY) * 1000);

    if (config_get(CONFIG_RAMP)) {
        start_ramp();
    } else {
        start_ground();
    }
}
