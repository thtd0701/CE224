#include "PID.h"

void PIDController_Init(PIDController *pid) {

    /* Clear controller variables */
    pid->T = 0.005f;
    pid->integrator = 0.0f;
    pid->prevError  = 0.0f;
    pid->prevMeasurement = 0.0f;
    pid->out = 0.0f;

}

float PIDController_Update(PIDController *pid, float setpoint, float measurement) {

    /* Error signal */
    float error = setpoint - measurement;

    /* Proportional */
    float proportional = pid->Kp * error;

    /* Integral */
    pid->integrator += pid->Ki * error * pid->T;

    /* Anti-wind-up via integrator clamping */
    if (pid->integrator > pid->limMaxInt) {
        pid->integrator = pid->limMaxInt;
    } else if (pid->integrator < pid->limMinInt) {
        pid->integrator = pid->limMinInt;
    }

    /* Derivative */
    float derivative = pid->Kd * (error - pid->prevError) / pid->T;

    /* Compute output and apply limits */
    pid->out = proportional + pid->integrator + derivative;

    if (pid->out > pid->limMax) {
        pid->out = pid->limMax;
    } else if (pid->out < pid->limMin) {
        pid->out = pid->limMin;
    }

    /* Store error for later use */
    pid->prevError = error;

    /* Return controller output */
    return pid->out;

}
