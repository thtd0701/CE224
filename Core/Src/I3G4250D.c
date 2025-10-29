#include "I3G4250D.h"

extern SPI_HandleTypeDef hspi1;

void I3G4250D_Write(uint8_t reg, uint8_t val)
{
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &reg, 1, HAL_MAX_DELAY);
    HAL_SPI_Transmit(&hspi1, &val, 1, HAL_MAX_DELAY);
    CS_HIGH();
}

void I3G4250D_ReadGyro(I3G4250D_GyroData *data)
{
    uint8_t addr = 0x28 | 0xC0;
    uint8_t buffer[6];
    CS_LOW();
    HAL_SPI_Transmit(&hspi1, &addr, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(&hspi1, buffer, 6, HAL_MAX_DELAY);
    CS_HIGH();
    data->x = (int16_t)(buffer[0] | (buffer[1] << 8));
    data->y = (int16_t)(buffer[2] | (buffer[3] << 8));
    data->z = (int16_t)(buffer[4] | (buffer[5] << 8));
    data->x_rps = (float)data->x * 0.00875f * 0.0174533f;
    data->y_rps = (float)data->y * 0.00875f * 0.0174533f;
    data->z_rps = (float)data->z * 0.00875f * 0.0174533f;
}

void I3G4250D_Init(void)
{
    I3G4250D_Write(0x20, 0x4F);
    I3G4250D_Write(0x21, 0x00);
    I3G4250D_Write(0x22, 0x00);
    I3G4250D_Write(0x23, 0x30);
    I3G4250D_Write(0x24, 0x00);
    I3G4250D_Write(0x25, 0x00);
}
