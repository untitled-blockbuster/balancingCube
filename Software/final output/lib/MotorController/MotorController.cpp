#include <MotorController.h>

MotorController::MotorController(
    int MotorEnablePin,
    int MotorDirPin,
    int MotorPWMPin,
    int AD0Pin,
    double Kp,
    double Ki,
    double Kd,
    float weightOffset // weight offset, it sames setpoint
)
{
    this->MotorEnablePin = MotorEnablePin;
    this->MotorDirPin = MotorDirPin;
    this->MotorPWMPin = MotorPWMPin;
    this->AD0Pin = AD0Pin;
    this->pid = PID(Kp, Ki, Kd, 0.02, -255, 255, -200, 200);
    this->weightOffset = weightOffset;
    pinMode(MotorEnablePin, OUTPUT);
    pinMode(MotorDirPin, OUTPUT);
    pinMode(AD0Pin, OUTPUT);
    analogWriteFrequency(MotorPWMPin, MOTOR_FREQ);
    digitalWrite(MotorEnablePin, MOTOR_DISABLE);
    digitalWrite(MotorDirPin, MOTOR_CW);
    analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
    digitalWrite(AD0Pin, LOW);
}
void MotorController::init()
{
    Wire.begin();
    Wire.setClock(400000);
    digitalWrite(AD0Pin, HIGH);
    mpu.initialize();
    int devStatus = mpu.dmpInitialize();
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    if (devStatus != 0)
    {
        Serial.printf("DMP init error (code: %d)\n", devStatus);
        digitalWrite(AD0Pin, LOW);
        this->dmpReady = false;
        return;
    }
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    mpu.setDMPEnabled(true);

    this->dmpReady = true;
    digitalWrite(AD0Pin, LOW);
}
void MotorController::updateSensor()
{
    if (dmpReady == false)
    {
        state = STATE_DMP_NOT_READY;
        return;
    }
    digitalWrite(AD0Pin, HIGH);
    if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
    {
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        hq = q.getProduct(offset);
        mpu.dmpGetGravity(&gravity, &hq);
        mpu.dmpGetYawPitchRoll(YPR, &hq, &gravity);
        angle = abs(YPR[0] * 180.f / M_PI);

        // 튀는값 제거
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
    if (state == STATE_NORMAL)
    {
        time = this->getTime();
        long double delta = time - prevTime;
        digitalWrite(MotorEnablePin, MOTOR_ENABLE);
        pid.Update(SETPOINT + weightOffset, angle, delta);
        bool MotorDir;
        if (pid.out > 0)
            MotorDir = MOTOR_CW;
        else
            MotorDir = MOTOR_ACW;
        digitalWrite(MotorDirPin, MotorDir);
        analogWrite(MotorPWMPin, MOTOR_SPD_MIN - abs(pid.out));

        prevTime = time;
    }
}

void MotorController::stopMotor()
{
    analogWrite(MotorPWMPin, MOTOR_SPD_MIN);
    digitalWrite(MotorEnablePin, MOTOR_DISABLE);
}

void MotorController::setOffset()
{
    offset = q.getConjugate();
}
