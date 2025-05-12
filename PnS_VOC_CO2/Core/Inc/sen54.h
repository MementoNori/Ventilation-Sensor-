/*
 * sen54.h
 *
 *  Created on: May 2, 2025
 *      Author: em_paula_iv
 */

#ifndef INC_SEN54_H_
#define INC_SEN54_H_

#include "stm32l4xx_hal.h"  // or your MCU-specific HAL header

#define SEN54_I2C_ADDR (0x69 << 1)  // 7-bit address shifted left by 1 for STM32 HAL


HAL_StatusTypeDef sen54_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef sen54_read_measurements(I2C_HandleTypeDef *hi2c, uint8_t *buffer, uint16_t len);

#endif /* INC_SEN54_H_ */
