/**
 * @file
 * Servo control interface for common use cases
 */

typedef struct {
    int port;
    int p_open, p_close;
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
 * Set the given servo to its open setting.
 */
void servo_open(servo_s *servi) {
    servo[servi->port] = servi->p_open;
}

/**
 * Set the given servo to its closed setting.
 */
void servo_close(servo_s *servi) {
    servo[servi->port] = servi->p_close;
}

bool servo_check(servo_s *servi) {
    return false;
}
