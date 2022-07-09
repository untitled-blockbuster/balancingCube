#ifndef MotorController_H
#define MotorController_H

#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <PID.h>

#define SETPOINT 45
#define DEADZONE 30
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
        double Kp,
        double Ki,
        double Kd,
        float weightOffset);
    void init();
    void updateSensor();
    void updateMotor();
    void stopMotor();
    void setOffset();

private:
    int AD0Pin;
    int MotorEnablePin;
    int MotorDirPin;
    int MotorPWMPin;
    float weightOffset;
    long double lastResetTime;
    long double prevTime;
    long double time;
    float prevAngle;
    uint8_t fifoBuffer[64];
    MPU6050 mpu = MPU6050(0x69);
    bool dmpReady;
    Quaternion q;
    Quaternion offset;
    float YPR[3];
    float angle;
    Quaternion hq;
    VectorFloat gravity;
    PID pid;
    long double getTime() { return micros() * 0.000001; }
};

#endif