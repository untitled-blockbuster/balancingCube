#include "PID.h"
PID::PID()
{
	this->Kp = 0;
	this->Ki = 0;
	this->Kd = 0;
	this->tau = 0;
	this->limMin = 0;
	this->limMax = 0;
	this->limMinInt = 0;
	this->limMaxInt = 0;
	Init();
}
PID::PID(
	float Kp,
	float Ki,
	float Kd,
	float tau,
	float limMin,
	float limMax,
	float limMinInt,
	float limMaxInt)
{
	this->Kp = Kp;
	this->Ki = Ki;
	this->Kd = Kd;
	this->tau = tau;
	this->limMin = limMin;
	this->limMax = limMax;
	this->limMinInt = limMinInt;
	this->limMaxInt = limMaxInt;
	Init();
}

void PID::Init()
{
	/* Clear controller variables */
	integrator = 0;
	prevError = 0;

	differentiator = 0;
	prevMeasurement = 0;

	out = 0;
}

void PID::Update(float setpoint, float measurement, float T)
{
	float error = setpoint - measurement;

	/*\
	|*|	Proportional
	\*/
	float proportional = Kp * error;

	/*\
	|*|	Integral
	\*/
	// apply clamping
	if ((error < 0 && integrator > 0) ||
		(error > 0 && integrator < 0))
		integrator = 0;
	// integrator += Ki * error;
	integrator = integrator + 0.5f * Ki * T * (error + prevError);
	if (integrator > limMaxInt)
		integrator = limMaxInt;
	else if (integrator < limMinInt)
		integrator = limMinInt;

	/*\
	|*|	Derivative (band-limited differentiator)
	\*/
	differentiator = (error - prevError) / T * Kd;
	// differentiator = -(2.0f * Kd * (measurement - prevMeasurement) // Note: derivative on measurement, therefore minus sign in front of equation!
	// 				   + (2.0f * tau - T) * differentiator) /
	// 				 (2.0f * tau + T);

	/*\
	|*|	Compute output and apply limits
	\*/
	out = proportional + integrator + differentiator;
	if (out > limMax)
		out = limMax;
	else if (out < limMin)
		out = limMin;

	/*\
	|*|	update memory
	\*/
	prevError = error;
	prevMeasurement = measurement;
}