typedef struct {
    int port;
    int p_open, p_close;
} servo_s;

void servo_init(servo_s *servo, int port, int up, int down) {
    servo->port = port;
    servo->p_open = up;
    servo->p_close = down;
}

void servo_open(servo_s *servi) {
    servo[servi->port] = servi->p_open;
}

void servo_close(servo_s *servi) {
    servo[servi->port] = servi->p_close;
}

bool servo_check(servo_s *servi) {
    return false;
}
