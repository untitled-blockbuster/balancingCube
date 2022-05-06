#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "PID.h"

#define SETPOINT 45
#define DEADPOINT 30
#define BREAK 2
#define PWM_OUT 5
#define REVERSE 4
#define FREQ 20000

PIDController pid = {
    57.4,   // Kp
    245.6,  // Ki
    0.05,   // Kd
    0.02,   // tau
    -255,   // PID limit min
    255,    // PID limit max
    -1,     // pid limit min int
    1,      // pid limit max int
};

long double prevTime = 0;
long double time = 0;
long double lastResetTime = 0;

MPU6050 mpu;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;        // [w, x, y, z]         quaternion container
float euler[3];      // [psi, theta, phi]    Euler angle container

void setup()
{
    PIDController_Init(&pid);

    analogWriteFrequency(PWM_OUT, FREQ);
    // pinMode(BREAK, OUTPUT);
    pinMode(REVERSE, OUTPUT);

    Wire.begin();
    Wire.setClock(400000);
    Serial.begin(115200);
    while (!Serial)
        ;

    mpu.initialize();
    delay(100);

    while (Serial.available() && Serial.read());

    devStatus = mpu.dmpInitialize();

    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus != 0)
    {
        Serial.printf("DMP init error (code: %d)\n", devStatus);
        return;
    }

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
}

void loop()
{
    if (!dmpReady)
        return;

    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        long double delta = time - prevTime;
        float angle = euler[2] * 180 / M_PI;

        Serial.printf("p: %f\ti: %f\td: %f\n", pid.prevError * pid.Kp, pid.integrator * pid.Ki, pid.differentiator);
        Serial.printf("Kp: %f\tKi: %f\tKd: %f\tout: %f\tangle: %f\tinteg: %f\n", pid.Kp, pid.Ki, pid.Kd, pid.out, angle, pid.integrator);
        Serial.println();

        // if (angle <= DEADPOINT || abs(angle) >= 90 - DEADPOINT)
        if(angle <= 35 || angle >= 115)
        {
            // 제어 가능범위 벗어남
            analogWrite(PWM_OUT, 255);
            digitalWrite(BREAK, LOW);
            return;
        }
        else {
            digitalWrite(BREAK, HIGH);
        }

        PIDController_Update(&pid, SETPOINT, angle, delta);
        digitalWrite(REVERSE, pid.out > 0);         // CW / CCW control
        analogWrite(PWM_OUT, 255.0 - abs(pid.out));   // motor speed control

        // to tuning PID
        if (Serial.available())
        {
            char input = Serial.read();
            switch (input)
            {
            case '1':
                pid.Kp += 0.1;
                break;
            case '2':
                pid.Kp -= 0.1;
                break;
            case '4':
                pid.Ki += 0.1;
                break;
            case '5':
                pid.Ki -= 0.1;
                break;

            case '7':
                pid.Kd += 0.01;
                break;
            case '8':
                pid.Kd -= 0.01;
                break;
            }
        }
    }

    if (time - lastResetTime > 30) {
        pid.integrator = 0;
        lastResetTime = time;
    }


    // time update
    prevTime = time;
    time = micros() / (long double)1000000;
}
