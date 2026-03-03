/*
 * mag3110.h
 *
 *  Created on: Mar 1, 2026
 *      Author: tomek
 */

#ifndef __MAG3110_H__
#define __MAG3110_H__

#include "stm32f1xx_hal.h"
#include <stdint.h>

/* 7-bit = 0x0E → HAL wymaga <<1 */
#define MAG3110_ADDR           (0x0E << 1)

/* rejestry */
#define MAG3110_DR_STATUS      0x00
#define MAG3110_OUT_X_MSB      0x01
#define MAG3110_WHO_AM_I       0x07
#define MAG3110_CTRL_REG1      0x10
#define MAG3110_CTRL_REG2      0x11

#define MAG3110_WHOAMI_VAL     0xC4

typedef struct
{
    I2C_HandleTypeDef *hi2c;
} MAG3110_t;

HAL_StatusTypeDef MAG3110_Init(MAG3110_t *dev, I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef MAG3110_ReadRaw(MAG3110_t *dev, int16_t *x, int16_t *y, int16_t *z);
HAL_StatusTypeDef MAG3110_Read_uT(MAG3110_t *dev, float *x, float *y, float *z);

#endif
