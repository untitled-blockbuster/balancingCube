#ifndef MotorController_H
#define motorController_H

#include <MPU6050_9Axis_MotionApps41.h>
#include <Servo.h>
#include <PID.h>

#define SETPOINT 45
#define MOTOR_FREQ 20000
#define MOTOR_ENABLE HIGH
#define MOTOR_DISABLE LOW
#define MOTOR_CW HIGH
#define MOTOR_ACW LOW
#define MOTOR_SPD_MAX 0
#define MOTOR_SPD_MIN 255
#define BREAK_ON 60
#define BREAK_OFF 0
#define STATE_STANDUP 1
#define STATE_NORMAL 2
#define STATE_DEAD 0

class MotorController
{
public:
    int state = 0;
    MotorController(int AD0Pin,
                    int MotorEnablePin,
                    int MotorDirPin,
                    int MotorPWMPin,
                    int BreakPin,
                    int Kp, int Ki, int Kd);
    void initialize();
    void update(long double T);
    double *getYRP() { return YRP; }
    double getYaw() { return YRP[0]; }
    double getRoll() { return YRP[1]; }
    double getPitch() { return YRP[2]; }

private:
    double YRP[3];
    int AD0Pin;
    int MotorEnablePin;
    int MotorDirPin;
    int MotorPWMPin;
    int BreakPin;
    long double prevTime;
    long double lastResetTime;
    MPU6050 mpu = MPU6050(0x69);
    Servo MotorBreak;
    PID pid;
    long double getTime() { return micros() * 0.000001; }
};

#endif