/*
 * mag3110.c
 *
 *  Created on: Mar 1, 2026
 *      Author: tomek
 */


#include "mag3110.h"

/* ===================== LOW LEVEL ===================== */

static HAL_StatusTypeDef readReg(MAG3110_t *dev, uint8_t reg, uint8_t *val)
{
    return HAL_I2C_Mem_Read(dev->hi2c, MAG3110_ADDR, reg,
                           I2C_MEMADD_SIZE_8BIT, val, 1, 100);
}

static HAL_StatusTypeDef writeReg(MAG3110_t *dev, uint8_t reg, uint8_t val)
{
    return HAL_I2C_Mem_Write(dev->hi2c, MAG3110_ADDR, reg,
                            I2C_MEMADD_SIZE_8BIT, &val, 1, 100);
}

/* ===================== INIT ===================== */

HAL_StatusTypeDef MAG3110_Init(MAG3110_t *dev, I2C_HandleTypeDef *hi2c)
{
    dev->hi2c = hi2c;

    uint8_t id;

    if(readReg(dev, MAG3110_WHO_AM_I, &id) != HAL_OK)
        return HAL_ERROR;

    if(id != MAG3110_WHOAMI_VAL)
        return HAL_ERROR;

    /* standby */
    writeReg(dev, MAG3110_CTRL_REG1, 0x00);

    /* auto reset */
    writeReg(dev, MAG3110_CTRL_REG2, 0x80);

    /* active + 80Hz + oversampling 16 */
    writeReg(dev, MAG3110_CTRL_REG1, 0x01);

    HAL_Delay(10);

    return HAL_OK;
}

/* ===================== READ RAW ===================== */

HAL_StatusTypeDef MAG3110_ReadRaw(MAG3110_t *dev, int16_t *x, int16_t *y, int16_t *z)
{
    uint8_t status;

    /* czekaj na gotowe dane */
    do
    {
        if(readReg(dev, MAG3110_DR_STATUS, &status) != HAL_OK)
            return HAL_ERROR;

    } while(!(status & 0x08));   // ZYXDR

    uint8_t buf[6];

    if(HAL_I2C_Mem_Read(dev->hi2c, MAG3110_ADDR,
                        MAG3110_OUT_X_MSB,
                        I2C_MEMADD_SIZE_8BIT,
                        buf, 6, 100) != HAL_OK)
        return HAL_ERROR;

    *x = (int16_t)((buf[0] << 8) | buf[1]);
    *y = (int16_t)((buf[2] << 8) | buf[3]);
    *z = (int16_t)((buf[4] << 8) | buf[5]);

    return HAL_OK;
}

/* ===================== READ uT ===================== */

HAL_StatusTypeDef MAG3110_Read_uT(MAG3110_t *dev, float *x, float *y, float *z)
{
    int16_t rx, ry, rz;

    if(MAG3110_ReadRaw(dev, &rx, &ry, &rz) != HAL_OK)
        return HAL_ERROR;

    /* 0.1 µT/LSB */
    *x = rx * 0.1f;
    *y = ry * 0.1f;
    *z = rz * 0.1f;

    return HAL_OK;
}
