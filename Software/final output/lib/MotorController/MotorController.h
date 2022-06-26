#ifndef MotorController_H
#define motorController_H

#include <MPU6050_6Axis_MotionApps20.h>
#include <Servo.h>
#include <PID.h>

#define SETPOINT 45
#define DEADZONE 40
#define MOTOR_FREQ 20000
#define MOTOR_ENABLE HIGH
#define MOTOR_DISABLE LOW
#define MOTOR_CW HIGH
#define MOTOR_ACW LOW
#define MOTOR_SPD_MAX 0
#define MOTOR_SPD_MIN 255
#define BREAK_ON 60
#define BREAK_OFF 0

#define STATE_NORMAL 1
#define STATE_DEAD 0
#define STATE_DMP_NOT_READY -1

class MotorController
{
public:
    int state = STATE_DMP_NOT_READY;
    MotorController(
        int MotorEnablePin,
        int MotorDirPin,
        int MotorPWMPin,
        int AD0Pin,
        int Kp, int Ki, int Kd);
    void init();
    void updateState();
    void updateMotor();

private:
    double YRP[3];
    int AD0Pin;
    int MotorEnablePin;
    int MotorDirPin;
    int MotorPWMPin;
    long double prevTime;
    bool dmpReady = false;
    float angle;
    float prevAngle;
    uint8_t fifoBuffer[64];
    MPU6050 mpu(0x69);
    PID pid;
    long double getTime() { return micros() * 0.000001; }
};

#endif