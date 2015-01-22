/**
 * @file
 * Servo control interface for common use cases
 */

#define SERVO_TIMER T1

int servo_timeout = 1000;   //< Servo move timeout in milliseconds
int servo_tolerance = 5;    //< Tolerance for servo target in degrees

typedef enum {
    SERVO_OK,               //< Servo ran without an error
    SERVO_NOCONFIG,         //< Servo used without configuration
    SERVO_TIMEOUT,          //< Servo timed out while moving in position
} servo_error;

typedef struct {
    int port;               //< Port number given by RobotC
    int p_open, p_close;    //< Open and closed position in degrees
} servo_s;

/**
 * Initialize the given servo structure with the given port number, open
 * setting, and closed setting.
 */
void servo_init(servo_s *servo, int port, int open, int closed) {
    servo->port = port;
    servo->p_open = open;
    servo->p_close = closed;
}

/**
 * Block until the given servo reaches its target, or a timeout occurs.
 */
static servo_error servo_wait(servo_s *srv, int target) {
    ClearTimer(SERVO_TIMER);

    while (abs(ServoValue[srv->port] - target) > servo_tolerance) {
        if (time1[SERVO_TIMER] > servo_timeout) {
            writeDebugStreamLine("[servo] Timeout at %d for target %d",
                    ServoValue[srv->port], target);
            return SERVO_TIMEOUT;
        }

        Sleep(250);
    }

    return SERVO_OK;
}

/**
 * Command the given servo to turn to its open setting. This function does
 * not block; use servo_open_wait() if blocking is required.
 */
void servo_open(servo_s *srv) {
    servo[srv->port] = srv->p_open;
}

/**
 * Command the given servo to turn to its closed setting. This function does
 * not block; use servo_close_wait() if blocking is required.
 */
void servo_close(servo_s *srv) {
    servo[srv->port] = srv->p_close;
}

/**
 * Turn the given servo and wait until it reaches its open setting.
 */
servo_error servo_open_wait(servo_s *srv) {
    servo_open(srv);
    return servo_wait(srv, srv->p_open);
}

/**
 * Turn the given servo and wait until it reaches its closed setting.
 */
servo_error servo_close_wait(servo_s *srv) {
    servo_close(srv);
    return servo_wait(srv, srv->p_close);
}
