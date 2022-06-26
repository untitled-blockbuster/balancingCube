#include <MotorController.h>

MotorController::MotorController(
    int MotorEnablePin,
    int MotorDirPin,
    int MotorPWMPin,
    int AD0Pin,
    int Kp, int Ki, int Kd)
{
    this->MotorEnablePin = MotorEnablePin;
    this->MotorDirPin = MotorDirPin;
    this->MotorPWMPin = MotorPWMPin;
    this->AD0Pin = AD0Pin;
    this->pid = PID(Kp, Ki, Kd, 0.02, -255, 255, -127, 127);
    pinMode(MotorEnablePin, OUTPUT);
    pinMode(MotorDirPin, OUTPUT);
    analogWriteFrequency(MotorPWMPin, MOTOR_FREQ);
    digitalWrite(MotorEnablePin, MOTOR_DISABLE);
    digitalWrite(MotorDirPin, MOTOR_CW);
    analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
}
void MotorController::init()
{
    digitalWrite(AD0Pin, HIGH);
    delay(100);
    int devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (!devStatus)
    {
        Serial.printf("DMP init error (code: %d)\n", devStatus);
        digitalWrite(AD0Pin, LOW);
        return;
    }

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    dmpReady = true;
    digitalWrite(AD0Pin, LOW);
}
void MotorController::updateState()
{
    if (!dmpReady)
    {
        state = STATE_DMP_NOT_READY;
        return;
    }
    digitalWrite(AD0Pin, HIGH);
    if (mpu.dmpGetCurrntFIFOPacket(fifoBuffer))
    {
        Quaternion q;   // [w, x, y, z]         quaternion container
        float euler[3]; // [psi, theta, phi]    Euler angle container
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);
        angle = euler[2] * 180 / M_PI;
        if (abs(angle - prevAngle) > 20)
            angle = prevAngle;

        if (DEADZONE < angle && ((90 - DEADZONE) > angle))
            state = STATE_NORMAL;
        else
            state = STATE_DEAD;
        prevAngle = angle;
    }
    digitalWrite(AD0Pin, LOW);
}
void MotorController::updateMotor()
{
    long double time = getTime();
    long double delta = time - prevTime;
    switch (state)
    {
    case STATE_DEAD:
        analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
        digitalWRite(MotorEnablePin, MOTOR_DISABLE);
        break;
    case STATE_NORMAL:
        pid.Update(SETPOINT, angle, delta);
        bool MotorDir;
        if (pid.out > 0)
            MotorDir = MOTOR_CW;
        else
            MotorDir = MOTOR_ACW;
        digitalWrite(MotorDirPin, MotorDir);
        analogWrite(MotorPWMPin, 255.f - abs(pid.out));
        break;
    }
    prevTime = time;
}