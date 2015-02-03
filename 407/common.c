#include "../lib/lib.h"

servo_s grabber_l;
servo_s grabber_r;
servo_s claw, scoop;

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
    servo_init(&grabber_l, sp_left, 120, 70);
    servo_init(&grabber_r, sp_right, 120, 170);
    servo_init(&claw, sp_claw, 170, 150);
    servo_init(&scoop, sp_scoop, 210, 0);
    grabber_up(false);
    servo_close(&claw);
    servo_close(&scoop);
    servo[sp_claw] = 135;
}
