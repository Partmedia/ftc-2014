#include "../libdrive.c"
#include "../libservo.c"

servo_s grabber_l;
servo_s grabber_r;

/**
 * Move the grabber to the up position.
 * @todo Put actual values here.
 */
void grabber_up(bool wait) {
    if (wait) {
        servo_open_wait(&grabber_l);
        servo_open_wait(&grabber_r);
    } else {
        servo_open(&grabber_l);
        servo_open(&grabber_r);
    }
}

/**
 * Move the grabber to the down position.
 * @todo Put actual values here.
 */
void grabber_down(bool wait) {
    if (wait) {
        servo_close_wait(&grabber_l);
        servo_close_wait(&grabber_r);
    } else {
        servo_close(&grabber_l);
        servo_close(&grabber_r);
    }
}

void init_common() {
    drive_init(m_left, m_right);
    servo_init(&grabber_l, sgl_port, 30, 200);
    servo_init(&grabber_r, sgr_port, 220, 70);
    grabber_up(false);
}
