servo_s servo_right, servo_left, servo_gate;

void grabber_up() {
    servo_open(&servo_left);
    servo_open(&servo_right);
}

void grabber_down() {
    servo_close(&servo_left);
    servo_close(&servo_right);
}

void init() {
    drive_init(m_left, m_right);
    servo_init(&servo_right, sp_grabber_r, 20, 165);
    servo_init(&servo_left, sp_grabber_l, 200, 60);
    servo_init(&servo_gate, gate, 80, 200);
    grabber_up();
    servo_close(&servo_gate);
}
