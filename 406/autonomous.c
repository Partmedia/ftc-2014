#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  motorA,          winder,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  mtr_S4_C1_1,     Squid,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C1_2,     Lever,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_1,     MotorB,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C2_2,     Right,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C3_1,     Rack,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S4_C3_2,     Left,          tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S4_C4_1,    Grabber,              tServoStandard)
#pragma config(Servo,  srvo_S4_C4_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S4_C4_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "../lib/drive.c"

task main() {
    waitForStart();
    drive_init(Left, Right);

    //Slightly raise arm/lever.
    motor[Lever] = 20;
    wait1Msec(1000);
    motor[Lever] = 0;

    playSound(soundBeepBeep);

    //Drive down ramp.
    drive_straight(10, 5000);

    playSound(soundBlip);

    //Raise arm/lever even more.
    motor[Lever] = 40;
    wait1Msec(2750);
    motor[Lever] = 0;

    playSound(soundDownwardTones);

    //Push goal to wall.
    drive_straight(25, 6000);

    playSound(soundBlip);

    //Slight turn right.
    drive_turn(-50, 500);

    playSound(soundBlip);

    //Move ball up.
    motor[winder] = 100;
    wait1Msec(7000);
    motor[winder] = 0;

    playSound(soundDownwardTones);

    //Dispense ball.
    motor[Squid] = -20;
    wait1Msec(6000);
    motor[Squid] = 0;

    playSound(soundUpwardTones);
}
