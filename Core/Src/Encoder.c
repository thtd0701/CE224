#include "Encoder.h"
#include <math.h>

// Motor specs: 333 RPM, 1:30 gear ratio, 11 PPR
#define MOTOR_PPR         11.0f      // Pulses Per Revolution (motor shaft)
#define GEAR_RATIO        30.0f      // 1:30 gear reduction
#define WHEEL_PPR         (MOTOR_PPR * GEAR_RATIO)  // 330 PPR at wheel
#define SAMPLE_TIME_MS    5.0f      // Sampling interval in milliseconds
#define SAMPLE_TIME_SEC   (SAMPLE_TIME_MS / 1000.0f)

static TIM_HandleTypeDef *encoder_left;
static TIM_HandleTypeDef *encoder_right;
static int32_t prev_left_count = 0;
static int32_t prev_right_count = 0;
static EncoderData current_speed = {0};

/**
 * @brief Initialize encoder timers
 * @param htim_left: TIM3 handle (left motor encoder)
 * @param htim_right: TIM4 handle (right motor encoder)
 */
void Encoder_Init(TIM_HandleTypeDef *htim_left, TIM_HandleTypeDef *htim_right)
{
  encoder_left = htim_left;
  encoder_right = htim_right;
  
  // Start encoder mode on both timers
  HAL_TIM_Encoder_Start(encoder_left, TIM_CHANNEL_ALL);
  HAL_TIM_Encoder_Start(encoder_right, TIM_CHANNEL_ALL);
  
  // Reset initial counts
  __HAL_TIM_SET_COUNTER(encoder_left, 0);
  __HAL_TIM_SET_COUNTER(encoder_right, 0);
  prev_left_count = 0;
  prev_right_count = 0;
}

/**
 * @brief Calculate motor speed from encoder counts
 * @return EncoderData structure with speeds in RPM
 */
void Encoder_GetSpeed(EncoderData* speed_data)
{
  // Read current encoder counts (16-bit values: 0-65535)
  int32_t left_count = __HAL_TIM_GET_COUNTER(encoder_left);
  int32_t right_count = __HAL_TIM_GET_COUNTER(encoder_right);
  
  // Calculate pulse differences since last reading
  int32_t left_pulses = left_count - prev_left_count;
  int32_t right_pulses = right_count - prev_right_count;
  
  // Handle 16-bit counter overflow (wrapping from 65535 to 0)
  // If difference is very negative, counter wrapped around
  if (left_pulses < -32767)
    left_pulses += 65536;
  if (right_pulses < -32767)
    right_pulses += 65536;
  
  // Handle reverse rotation (negative counts)
  if (left_pulses > 32767)
    left_pulses -= 65536;
  if (right_pulses > 32767)
    right_pulses -= 65536;
  
  // Convert pulses to RPM
  // RPM = (pulses / PPR) * (60 / time_in_seconds)
  speed_data->left_speed = (left_pulses / WHEEL_PPR) * (60.0f / SAMPLE_TIME_SEC);
  speed_data->right_speed = (right_pulses / WHEEL_PPR) * (60.0f / SAMPLE_TIME_SEC);
  
  // Calculate speed difference and average
  speed_data->speed_diff = speed_data->left_speed - speed_data->right_speed;
  speed_data->avg_speed = (speed_data->left_speed + speed_data->right_speed) / 2.0f;
  
  // Store raw counts
  speed_data->left_count = left_count;
  speed_data->right_count = right_count;
  
  // Update previous counts for next iteration
  prev_left_count = left_count;
  prev_right_count = right_count;
}

/**
 * @brief Reset encoder counts to zero
 */
void Encoder_Reset(void)
{
  __HAL_TIM_SET_COUNTER(encoder_left, 0);
  __HAL_TIM_SET_COUNTER(encoder_right, 0);
  prev_left_count = 0;
  prev_right_count = 0;
}
