#include <Arduino.h>
#include <I2Cdev.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Wire.h>
#include <PID.h>
#include <Servo.h>
#include <Queue.h>

/*\
|*| 핀 설정
\*/
#define REVERSE 4 // 모터 정/역제어
#define PWM_OUT 5 // 모터 속력제어

#define BREAK_BASIC 2 // 모터 자체내장 브레이크
#define BREAK_EXTRA 6 // 외부 브레이크

#define STANDUP_BTN 1 // 외부 버튼, 누를 경우 데드존에서 스스로 일어섬

/*\
|*| 기타 상수들, 필요에 따라 수정
\*/
#define DEADPOINT 30 // 넘어진 상태로 간주할 각도, 0 ~ 45

/*\
|*| 이하는 시스템 상수
|*| 가급적 수정을 피할것
\*/
#define SETPOINT 45
#define RUBBER_BREAK_PIN 6
#define BREAK_RUBBER 6
#define BREAK_ON 50
#define BREAK_OFF 0
#define BREAK_DURATION 0.7
#define STANDUP_BTN 1
#define PWM_OUT 5
#define REVERSE 4
#define FREQ 20000

// 시리얼 출력옵션
#define OUTPUT_BASIC_INFO
#define OUTPUT_SERIAL_PLOTTER

// 시간변수
long double time = 0;
long double prevTime = 0;
long double lastResetTime = 0;

PID pid = PID(
    57.4,  // Kp
    245.6, // Ki
    0.06,  // Kd
    0.02,  // tau
    -255,  // PID limit min
    255,   // PID limit max
    -1,    // pid limit min int
    1      // pid limit max int
);

Servo breakMotor;

MPU6050 mpu;

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Queue filter = Queue(10); // sensor filter

Quaternion q;   // [w, x, y, z]         quaternion container
float euler[3]; // [psi, theta, phi]    Euler angle container

long double getSecond()
{
    return micros() * 0.000001;
}

void setup()
{
    pinMode(BREAK_BASIC, OUTPUT);
    pinMode(REVERSE, OUTPUT);
    pinMode(STANDUP_BTN, INPUT_PULLDOWN);

    digitalWrite(BREAK_BASIC, HIGH);
    digitalWrite(REVERSE, HIGH);

    analogWriteFrequency(PWM_OUT, FREQ);

    breakMotor.attach(BREAK_RUBBER);
    breakMotor.write(BREAK_OFF);

    Serial.begin(115200);

    while (!Serial)
        ;

    Wire.begin();

    Wire.setClock(400000);

    mpu.initialize();
    delay(100);

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

#ifdef OUTPUT_BASIC_INFO
        Serial.printf("p: %f\ti: %f\td: %f\n", pid.prevError * pid.Kp, pid.integrator * pid.Ki, pid.differentiator);
        Serial.printf("Kp: %.4f\tKi: %.4f\tKd: %.4f\tout: %.4f\tangle: %.4f\tinteg: %.4f\tdelta: %f\n", pid.Kp, pid.Ki, pid.Kd, pid.out, angle, pid.integrator, delta);
        Serial.println();
#endif

#ifdef OUTPUT_SERIAL_PLOTTER
        // converted 0 ~ 100
        Serial.println("angle,PIDOUT,integ,min,max");
        Serial.printf("%.2f,", (abs(angle) > 90 ? 90 : angle) * 10 / 9);
        Serial.printf("%.2f,", (255.0 - pid.out) * 50 / 255);
        Serial.printf("%.2f,", (pid.integrator + 1) * 50);
        Serial.printf("%.2f,", 0.0);
        Serial.printf("%.2f\n", 100.0);
#endif

        // 제어 가능범위 벗어남
        if (angle <= DEADPOINT || angle >= 90 - DEADPOINT)
        {
            // 브레이크를 이용하여 세우기 동작
            if (digitalRead(STANDUP_BTN))
            {
                // 각도값으로 정/역제어 이후, 가속
                digitalWrite(REVERSE, angle > SETPOINT);
                analogWrite(PWM_OUT, 0);

                // 5초 대기(가속시간 대기)
                long double delayTime = getSecond();
                while (delayTime - time < 5)
                {
                    delayTime = getSecond();
                }

                // 브레이크
                analogWrite(PWM_OUT, 255);
                digitalWrite(BREAK_BASIC, LOW);
                breakMotor.write(BREAK_ON);

                // n초간 브레이크
                time = delayTime = getSecond();
                while (delayTime - time < BREAK_DURATION)
                {
                    delayTime = getSecond();
                }

                //브레이크 해제 이후 계속해서 PID제어
                digitalWrite(BREAK_BASIC, HIGH);
                breakMotor.write(BREAK_OFF);
                time = getSecond();
            }
            else
            {
                analogWrite(PWM_OUT, 255);
                return;
            }
        }
        else
        {
            digitalWrite(BREAK_BASIC, HIGH);
        }

        pid.Update(SETPOINT, angle, delta);
        digitalWrite(REVERSE, pid.out > 0);         // CW / CCW control
        analogWrite(PWM_OUT, 255.0 - abs(pid.out)); // motor speed control
    }

    if (time - lastResetTime > 30)
    {
        pid.integrator = 0;
        lastResetTime = time;
    }

    // time update
    prevTime = time;
    time = getSecond();
}