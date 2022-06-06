#include <MotorController.h>

MotorController::MotorController(int AD0Pin,
                                 int MotorEnablePin,
                                 int MotorDirPin,
                                 int MotorPWMPin,
                                 int BreakPin,
                                 int Kp, int Ki, int Kd)
{
    this->AD0Pin = AD0Pin;
    this->MotorEnablePin = MotorEnablePin;
    this->MotorDirPin = MotorDirPin;
    this->MotorPWMPin = MotorPWMPin;
    this->BreakPin = BreakPin;
    this->pid = PID(
        Kp, Ki, Kd, 0.02, -255, 255);
    pinMode(AD0Pin, OUTPUT);
    pinMode(MotorEnablePin, OUTPUT);
    pinMode(MotorEnablePin, MOTOR_DISABLE);
    pinMode(MotorDirPin, OUTPUT);
    analogWriteFrequency(MotorPWMPin, MOTOR_FREQ);
    analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
    MotorBreak.attach(BreakPin);
    MotorBreak.write(BREAK_OFF);
}

void MotorController::initialize()
{
    digitalWrite(AD0Pin, HIGH);
    mpu.initialize();
    uint8_t devStatus = mpu.dmpInitialize();
    if (devStatus != 0)
    {
        Serial.printf("DMP init error (code: %d\n", devStatus);
        while (1)
            continue;
    }
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.setDMPEnabled(true);
    digitalWrite(AD0Pin, LOW);
}

void MotorController::update()
{
    digitalWrite(AD0Pin, HIGH);
    switch (state)
    {
    case STATE_DEAD:
        analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
        digitalWRite(MotorEnablePin, MOTOR_DISABLE);
        break;
    case STATE_STANDUP:
        break;
    case STATE_NORMAL:
        break;
    }
    digitalWrite(AD0Pin, LOW);
}