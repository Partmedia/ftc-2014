#pragma config(Hubs,  S4, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S1,     IR,             sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S4,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,          winder,        tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorB,          NA,            tmotorNXT, PIDControl, encoder)
#pragma config(Motor,  motorC,          NA,            tmotorNXT, PIDControl, encoder)
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

    //Drive for certain amount of time.
    drive_straight(20, 9000);

    playSound(soundBlip);

    //Raise arm.
    motor[Rack] = 25;
    wait1Msec(5000);
    motor[Rack] = 0;

    playSound(soundDownwardTones);

    //Raise grabber.


    //Dispense ball.
    //motor[winder] = 50;
    //wait1Msec(1000);
    //motor[winder] = 0;

    //playSound(soundException);

}
