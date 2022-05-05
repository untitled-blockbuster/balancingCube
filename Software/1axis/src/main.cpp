#include <Arduino.h>
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include "Wire.h"
#include "PID.h"

#define OUTPUT_READABLE_QUATERNION
#define INTERRUPT_PIN 2
#define SETPOINT -45.0
#define DEADPOINT 30
#define BREAK 2
#define PWM_OUT 5
#define REVERSE 4
#define FREQ 20000

PIDController pid = {
    1,    // Kp
    0,  // Ki
    0,   // Kd
    0.02, // tau
    -255, // PID limit min
    255,  // PID limit max
    -127, // pid limit min int
    127,  // pid limit max int
};
long double prevTime = 0;
long double time = 0;

MPU6050 mpu;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;        // [w, x, y, z]         quaternion container
VectorInt16 aa;      // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;  // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3];      // [psi, theta, phi]    Euler angle container
float ypr[3];        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

uint8_t teapotPacket[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n'};

volatile bool mpuInterrupt = false;
void dmpDataReady()
{
    mpuInterrupt = true;
}

void setup()
{
    PIDController_Init(&pid);

    analogWriteFrequency(PWM_OUT, FREQ);
    pinMode(BREAK, OUTPUT);
    pinMode(REVERSE, OUTPUT);
    digitalWrite(BREAK, HIGH);

    Wire.begin();
    Wire.setClock(400000);
    Serial.begin(115200);
    while (!Serial)
        ;

    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    while (Serial.available() && Serial.read())
        ;

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

    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
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
        // Processing connection
        mpu.dmpGetQuaternion(&q, fifoBuffer); /*
         Serial.print(q.w + 1, 4);
         Serial.print(",");
         Serial.print(q.x + 1, 4);
         Serial.print(",");
         Serial.print(q.y + 1, 4);
         Serial.print(",");
         Serial.print(q.z + 1, 4);
         Serial.print(",");
         Serial.print("\n");*/

        mpu.dmpGetEuler(euler, &q);
        // deltaTime = (time - prevTime)
        long double delta = time - prevTime;
        float angle = euler[0] * 180 / M_PI;
        if (abs(angle) <= DEADPOINT || abs(angle) >= 90 - DEADPOINT)
        {
            // 제어 가능범위 벗어남
            analogWrite(PWM_OUT, 255);
            digitalWrite(BREAK, LOW);
            // Serial.println("DEAD");
            return;
        }
        digitalWrite(BREAK, HIGH);
        PIDController_Update(&pid, SETPOINT, angle, delta);
        digitalWrite(REVERSE, pid.out < 0);
        analogWrite(PWM_OUT, 255 - abs(pid.out));
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
            }
        }
        Serial.printf("p: %f\ti: %f\td: %f\n", pid.prevError * pid.Kp, pid.integrator * pid.Ki, pid.differentiator);
        //Serial.printf("error: %f\tprevError: %f\tdeltaTime: %d\tKd: %f\n", SETPOINT-angle, pid.prevError, delta, pid.Kd);
        //Serial.printf("time: %f\tprevTime: %f\n", time, prevTime);
        Serial.printf("Kp: %f\tKi: %f\tKd: %f\tout: %f\tangle: %f\tinteg: %f\n", pid.Kp, pid.Ki, pid.Kd, pid.out, angle, pid.integrator);
        Serial.println();
    }
    prevTime = time;
    time = micros() / (long double)1000000;
}
