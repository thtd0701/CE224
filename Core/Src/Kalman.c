#include "Kalman.h"
#include <math.h>
#include <string.h>

void Kalman_Init(KalmanFilter *kf, float dt)
{
  kf->dt = dt;
  
  // Initialize state [roll, bias]
  kf->x[0] = 0.0f;  // roll angle
  kf->x[1] = 0.0f;  // gyro bias
  
  // Initialize covariance matrix P
  kf->P[0][0] = 1.0f;
  kf->P[0][1] = 0.0f;
  kf->P[1][0] = 0.0f;
  kf->P[1][1] = 1.0f;
  
  // Process noise covariance Q
  kf->Q[0][0] = 0.01f;   // Process noise for angle
  kf->Q[0][1] = 0.0f;
  kf->Q[1][0] = 0.0f;
  kf->Q[1][1] = 0.01f;   // Process noise for bias
  
  // Measurement noise variance R
  kf->R = 10.0f;  // Accelerometer noise
  
  kf->z = 0.0f;
}

void Kalman_Predict(KalmanFilter *kf, float gyroRate)
{
  // Predict state: x = A*x + B*u
  // x[0] = x[0] + (gyroRate - x[1]) * dt
  // x[1] = x[1]
  float gyroUnbiased = gyroRate - kf->x[1];
  kf->x[0] += gyroUnbiased * kf->dt;
  
  // Predict covariance: P = A*P*A' + Q
  // A = [1, -dt; 0, 1]
  float P00 = kf->P[0][0];
  float P01 = kf->P[0][1];
  float P10 = kf->P[1][0];
  float P11 = kf->P[1][1];
  
  kf->P[0][0] = P00 - P01 * kf->dt - kf->dt * P10 + kf->dt * kf->dt * P11 + kf->Q[0][0];
  kf->P[0][1] = P01 - P11 * kf->dt;
  kf->P[1][0] = P10 - P11 * kf->dt;
  kf->P[1][1] = P11 + kf->Q[1][1];
}

void Kalman_Update(KalmanFilter *kf, float accelAngle)
{
  // Measurement residual
  float y = accelAngle - kf->x[0];
  
  // Kalman gain: K = P*H' / (H*P*H' + R)
  // H = [1, 0]
  float S = kf->P[0][0] + kf->R;  // Innovation covariance
  float K[2];
  K[0] = kf->P[0][0] / S;
  K[1] = kf->P[1][0] / S;
  
  // Update state: x = x + K*y
  kf->x[0] += K[0] * y;
  kf->x[1] += K[1] * y;
  
  // Update covariance: P = (I - K*H)*P
  // I - K*H = [[1 - K[0], -K[1]], [0, 1]]
  float P00 = (1.0f - K[0]) * kf->P[0][0];
  float P01 = (1.0f - K[0]) * kf->P[0][1] - K[1] * kf->P[1][1];
  float P10 = -K[0] * kf->P[1][0];
  float P11 = (1.0f - K[0]) * kf->P[1][0] - K[1] * kf->P[1][1];
  
  kf->P[0][0] = P00;
  kf->P[0][1] = P01;
  kf->P[1][0] = P10;
  kf->P[1][1] = P11;
}

float Kalman_GetRoll(KalmanFilter *kf)
{
  return kf->x[0];
}