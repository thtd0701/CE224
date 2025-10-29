/*
 * Madgwick.h
 *
 *  Created on: Oct 15, 2025
 *      Author: thtd0
 */

#ifndef INC_MADGWICK_H_
#define INC_MADGWICK_H_

void Madgwick_Init(float sampleFreq, float beta);
// IMU-only (6DOF) update: gyro (rad/s), accel (g)
void Madgwick_Update6DOF(float gx, float gy, float gz, float ax, float ay, float az);
void Madgwick_GetQuaternion(float *q0, float *q1, float *q2, float *q3);
void Madgwick_GetEuler(float q0, float q1, float q2, float q3, float *roll, float *pitch, float *yaw);

#endif /* INC_MADGWICK_H_ */
