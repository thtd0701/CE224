/*
 * I3G4250D.h
 *
 *  Created on: Oct 15, 2025
 *      Author: thtd0
 */

#ifndef INC_I3G4250D_H_
#define INC_I3G4250D_H_

#include "stm32f4xx_hal.h"
#include "stdint.h"

#define CS_LOW() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_SET)

typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
    float x_rps;   
    float y_rps;
    float z_rps;
} I3G4250D_GyroData;

void I3G4250D_Write(uint8_t reg, uint8_t val);
void I3G4250D_ReadGyro(I3G4250D_GyroData* data);
void I3G4250D_Init(void);
#endif /* INC_I3G4250D_H_ */
