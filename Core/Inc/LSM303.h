/*
 * LSM303.h
 *
 *  Created on: Oct 15, 2025
 *      Author: thtd0
 */

#ifndef INC_LSM303_H_
#define INC_LSM303_H_
#include "stm32f4xx_hal.h"
#include "stdint.h"

#define LSM303_ACCEL_ADDRESS 0x19 << 1 


typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
    float x_g;  
    float y_g;
    float z_g;
} LSM303_AccelData;


void LSM303_Write_Accel(uint8_t reg, uint8_t val);
void LSM303_ReadAccel(LSM303_AccelData* data);
void LSM303_Init(void);


#endif /* INC_LSM303_H_ */
