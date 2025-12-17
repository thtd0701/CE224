#ifndef KALMAN_H
#define KALMAN_H

#include <stdint.h>

typedef struct {
  // State vector [roll, bias]
  float x[2];
  
  // Covariance matrix P (2x2)
  float P[2][2];
  
  // Process noise covariance Q (2x2)
  float Q[2][2];
  
  // Measurement noise variance R
  float R;
  
  // Measurement from accelerometer
  float z;
  
  // Time step (seconds)
  float dt;
} KalmanFilter;

void Kalman_Init(KalmanFilter *kf, float dt);
void Kalman_Predict(KalmanFilter *kf, float gyroRate);
void Kalman_Update(KalmanFilter *kf, float accelAngle);
float Kalman_GetRoll(KalmanFilter *kf);

#endif // KALMAN_H