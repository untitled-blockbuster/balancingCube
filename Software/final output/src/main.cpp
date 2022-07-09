#include <Arduino.h>
#include <cube_1Axis.h>

unsigned int btn1 = 22;
unsigned int btn2 = 23;
Cube_1Axis oneAxis;
void setup()
{
    pinMode(btn1, INPUT_PULLDOWN);
    oneAxis.setup1Axis();
}
void loop()
{
    if (digitalRead(btn1))
        oneAxis.setup1Axis();
    else if (digitalRead(btn2))
    {
        oneAxis.setOffset();
    }
    oneAxis.loop1Axis();
}
