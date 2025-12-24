#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct {

    /* Controller gains */
    float Kp;
    float Ki;
    float Kd;

    /* Output limits */
    float limMin;
    float limMax;
    
    /* Integrator limits */
    float limMinInt;
    float limMaxInt;

    /* Sample time (in seconds) */
    float T;

    /* Controller "memory" */
    float integrator;
    float prevError;
    float prevMeasurement;

    /* Controller output */
    float out;

} PIDController;

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);

#endif
