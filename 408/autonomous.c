#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     m_right,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     m_left,        tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     motorF,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     lift,          tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C3_1,    gate,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    grabber_r,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    grabber_l,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/drive.c"

task main() {
    waitForStart();
    drive_init(m_left, m_right);
    bool on_ramp = true;  // whether robot is moving from ramp (vs. floor)
    if(on_ramp){
        drive_straight(-30, 3000);  // move backwards

        servo[grabber_l] = 80;  // move grabbers down
        servo[grabber_r] = 175;

        motor[lift] = 69;  // raise lift
        motor[gate] = 29;  // open gate
        wait1Msec(1000);
        motor[lift] = 0;
        motor[gate] = 0;
    }
    else{
        drive_turn(30, 3000);  // turn right
        drive_straight(-30, 3000);  // move backwards
        drive_turn(-30, 3000);  // turn left
        drive_straight(-30, 3000);  // move backwards

        servo[grabber_l] = 80;  // move grabbers down
        servo[grabber_r] = 175;

        motor[lift] = 69; // lift goes up
        motor[gate] = 29; // gate opens
        wait1Msec(1000); // wait
        motor[lift] = 0; // lift stops
        motor[gate] = 0; // gate stops
    }
}
