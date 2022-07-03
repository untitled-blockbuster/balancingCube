#include <cube_1Axis.h>

Cube_1Axis::Cube_1Axis() {}

void Cube_1Axis::setup1Axis()
{
    pinMode(orange_led, OUTPUT);
    pinMode(red_led, OUTPUT);
    digitalWrite(orange_led, LOW);
    digitalWrite(red_led, LOW);

    digitalWrite(orange_led, HIGH); // mpu init alram
    Serial.begin(115200);
    Serial.println("mpu init...");
    delay(1000);
    digitalWrite(orange_led, LOW);
    for (int i = 0; i < 3; ++i)
    {
        controller[i].init();
        // blink after mpu init
        digitalWrite(red_led, HIGH);
        delay(300);
        digitalWrite(red_led, LOW);
        delay(100);
    }
    Serial.println("all mpu init complete");

    digitalWrite(orange_led, HIGH); // setting offset alram
    Serial.println("offset setting...");
    delay(1000);
    digitalWrite(orange_led, LOW);
    double long offsetTime, ledBlinkTime, now;
    offsetTime = ledBlinkTime = now = micros() * 0.000001;
    bool ledState = true;
    while (offsetTime + 10 > now)
    {
        if (ledBlinkTime + 1 < now)
        {
            ledBlinkTime = now;
            digitalWrite(red_led, ledState);
            ledState = !ledState;
        }
        for (int i = 0; i < 3; i++)
            controller[i].updateSensor();

        now = micros() * 0.000001;
    }
    for (int i = 0; i < 3; i++)
        controller[i].setOffset();
    digitalWrite(red_led, LOW);
    Serial.println("all offset setting complete");
    for (int i = 0; i < 5; i++)
    { // blink orange led when finish init
        digitalWrite(orange_led, HIGH);
        delay(100);
        digitalWrite(orange_led, LOW);
        delay(100);
    }
    Serial.println("loop start");
}

void Cube_1Axis::loop1Axis()
{
    Serial.println("m1,m2,m3,o0");
    for (int i = 0; i < 3; i++)
    {
        if (controller[i].state == STATE_DMP_NOT_READY)
        {
            // alram error
            digitalWrite(red_led, HIGH);
            return;
        }
        controller[i].updateSensor();
    }
    Serial.println("0");
    for (int i = 0; i < 3; i++)
    {
        if (controller[i].state == STATE_DEAD)
            controller[i].stopMotor();
        else
        {
            controller[i].updateMotor();
            return;
        }
    }
}