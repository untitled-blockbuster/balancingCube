#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

class PID
{
private:
	void Init();

public:
	/* Controller gains */
	float Kp;
	float Ki;
	float Kd;

	/* Derivative low-pass filter time constant */
	float tau;

	/* Output limits */
	float limMin;
	float limMax;
	float limMaxInt;
	float limMinInt;

	/* Controller "memory" */
	float integrator;
	float prevError; /* Required for integrator */
	float differentiator;
	float prevMeasurement; /* Required for differentiator */

	/* Controller output */
	float out;

	PID();
	PID(
		float Kp,
		float Ki,
		float Kd,
		float tau,
		float limMin,
		float limMax,
		float limMinInt,
		float limMaxInt);

	void Update(float setpoint, float measurement, float T);
};
#endif
