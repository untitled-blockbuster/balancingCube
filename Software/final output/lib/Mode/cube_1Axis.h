#ifndef CUBE_1AXIS
#define CUBE_1AXIS

#include <MotorController.h>
#include <Wire.h>
class Cube_1Axis
{
public:
    Cube_1Axis();
    void setup1Axis();
    void loop1Axis();

private:
    unsigned int orange_led = 0;
    unsigned int red_led = 1;
    MotorController controller[3] = {
        MotorController( // Y axis
            7,           // motorEnable pin
            9,           // motorDir pin
            8,           // motorPWM pin
            17,          // mpu6050 AD0 pin
            90,          // Kp
            100,         // Ki
            0.006,       // Kd
            -2           // weight offset
            ),
        MotorController( // X axis
            4,           // motorEnable pin
            6,           // motorDir pin
            5,           // motorPWM pin
            20,          // mpu6050 AD0 pin
            90,          // Kp
            100,         // Ki
            0.006,       // Kd
            -2           // weight offset
            ),
        MotorController( // Z axis
            10,          // motorEnable pin
            12,          // motorDir pin
            11,          // motorPWM pin
            21,          // mpu6050 AD0 pin
            90,          // Kp
            100,         // Ki
            0.006,       // Kd
            -2           // weight offset
            )};
};

#endif