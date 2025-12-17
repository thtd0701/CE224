#ifndef ENCODER_H
#define ENCODER_H

#include "main.h"

typedef struct {
  float left_speed;      // RPM
  float right_speed;     // RPM
  float speed_diff;      // Left - Right (RPM)
  float avg_speed;       // Average speed (RPM)
  int32_t left_count;    // Raw encoder count
  int32_t right_count;   // Raw encoder count
} EncoderData;

// Function prototypes
void Encoder_Init(TIM_HandleTypeDef *htim_left, TIM_HandleTypeDef *htim_right);
void Encoder_GetSpeed(EncoderData* speed_data);
void Encoder_Reset(void);

#endif // ENCODER_H