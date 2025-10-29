/*
 * Madgwick.c
 *
 *  Created on: Oct 15, 2025
 *      Author: thtd0
 */

#include "Madgwick.h"
#include <math.h>

static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
static float beta = 0.1f;
static float invSampleFreq = 1.0f / 512.0f;

void Madgwick_Init(float sampleFreq, float beta_)
{
    q0 = 1.0f; q1 = q2 = q3 = 0.0f;
    beta = beta_;
    invSampleFreq = 1.0f / sampleFreq;
}

// IMU-only (6DOF) update: gyro (rad/s), accel (g)
void Madgwick_Update6DOF(float gx, float gy, float gz, float ax, float ay, float az)
{
    float recipNorm;
    float s0, s1, s2, s3;
    float qDot1, qDot2, qDot3, qDot4;
    float _2q0 = 2.0f * q0;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _4q0 = 4.0f * q0;
    float _4q1 = 4.0f * q1;
    float _4q2 = 4.0f * q2;
    float _8q1 = 8.0f * q1;
    float _8q2 = 8.0f * q2;
    float q0q0 = q0 * q0;
    float q1q1 = q1 * q1;
    float q2q2 = q2 * q2;
    float q3q3 = q3 * q3;

    // Normalize accelerometer measurement
    recipNorm = sqrtf(ax * ax + ay * ay + az * az);
    if (recipNorm == 0.0f) return;
    recipNorm = 1.0f / recipNorm;
    ax *= recipNorm;
    ay *= recipNorm;
    az *= recipNorm;

    // Gradient descent algorithm corrective step (IMU)
    s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
    s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
    s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
    s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
    recipNorm = 1.0f / sqrtf(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3);
    s0 *= recipNorm;
    s1 *= recipNorm;
    s2 *= recipNorm;
    s3 *= recipNorm;

    // Rate of change of quaternion from gyroscope
    qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
    qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
    qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
    qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

    // Integrate to yield quaternion
    q0 += (qDot1 - beta * s0) * invSampleFreq;
    q1 += (qDot2 - beta * s1) * invSampleFreq;
    q2 += (qDot3 - beta * s2) * invSampleFreq;
    q3 += (qDot4 - beta * s3) * invSampleFreq;

    // Normalize quaternion
    recipNorm = 1.0f / sqrtf(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
    q0 *= recipNorm;
    q1 *= recipNorm;
    q2 *= recipNorm;
    q3 *= recipNorm;
}

void Madgwick_GetQuaternion(float *out_q0, float *out_q1, float *out_q2, float *out_q3)
{
    *out_q0 = q0;
    *out_q1 = q1;
    *out_q2 = q2;
    *out_q3 = q3;
}

void Madgwick_GetEuler(float q0, float q1, float q2, float q3, float *roll, float *pitch, float *yaw)
{
    // Roll (x-axis rotation)
    *roll = atan2f(2.0f * (q0 * q1 + q2 * q3), 1.0f - 2.0f * (q1 * q1 + q2 * q2));
    // Pitch (y-axis rotation)
    float sinp = 2.0f * (q0 * q2 - q3 * q1);
    if (fabsf(sinp) >= 1)
        *pitch = copysignf(3.14159f / 2, sinp); // use 90 degrees if out of range
    else
        *pitch = asinf(sinp);
    // Yaw (z-axis rotation)
    *yaw = atan2f(2.0f * (q0 * q3 + q1 * q2), 1.0f - 2.0f * (q2 * q2 + q3 * q3));
}