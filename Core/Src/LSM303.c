/*
 * LSM303.c
 *
 *  Created on: Oct 15, 2025
 *      Author: thtd0
 */

#include "LSM303.h"

extern I2C_HandleTypeDef hi2c1;

void LSM303_Write_Accel(uint8_t reg, uint8_t val)
{
    HAL_I2C_Mem_Write(&hi2c1, LSM303_ACCEL_ADDRESS, reg, 1, &val, 1, HAL_MAX_DELAY);
}


void LSM303_ReadAccel(LSM303_AccelData *data)
{
    uint8_t addr = 0x28 | 0x80;
    uint8_t buffer[6];
    HAL_I2C_Mem_Read(&hi2c1, LSM303_ACCEL_ADDRESS, addr, 1, buffer, 6, HAL_MAX_DELAY);
    data->x = (int16_t)((buffer[0] | (buffer[1] << 8))) >> 6;
    data->y = (int16_t)((buffer[2] | (buffer[3] << 8))) >> 6;
    data->z = (int16_t)((buffer[4] | (buffer[5] << 8))) >> 6;
    data->x_g = (float)data->x * 0.0039f;
    data->y_g = (float)data->y * 0.0039f;
    data->z_g = (float)data->z * 0.0039f;
}



void LSM303_Init(void)
{
    LSM303_Write_Accel(0x20, 0x67);
    LSM303_Write_Accel(0x21, 0x00);
    LSM303_Write_Accel(0x22, 0x00);
    LSM303_Write_Accel(0x23, 0x80);
    LSM303_Write_Accel(0x24, 0x00);
    LSM303_Write_Accel(0x25, 0x00);
}
