/*
 * sen54.c
 *
 *  Created on: May 2, 2025
 *      Author: em_paula_iv
 */

#include "sen54.h"
#include "main.h"       // Optional: for global I2C handle if needed
#include <string.h>
#include <stdint.h>
#include "sensirion_i2c_hal.h"
#include "sen5x_i2c.h"
// SEN54 command to start measurement (as per datasheet)
static const uint8_t start_measurement_cmd[] = { 0x00, 0x10 };

// SEN54 command to read measured values
static const uint8_t read_measurement_cmd[] = { 0x03, 0xC4 };

// Initialization function: sends command to start measurements
HAL_StatusTypeDef sen54_init(I2C_HandleTypeDef *hi2c)
{
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(hi2c, SEN54_I2C_ADDR, (uint8_t*)start_measurement_cmd, sizeof(start_measurement_cmd), HAL_MAX_DELAY);
    HAL_Delay(100);  // Give sensor time to start measuring

    /*printf("Sending to I2C address 0x%02X\r\n", SEN54_I2C_ADDR);*/


    return status;
}

// Read raw measurement data into buffer
HAL_StatusTypeDef sen54_read_measurements(I2C_HandleTypeDef *hi2c, uint8_t *buffer, uint16_t len)
{
    HAL_StatusTypeDef status;

    // Request data from sensor
    status = HAL_I2C_Master_Transmit(hi2c, SEN54_I2C_ADDR, (uint8_t*)read_measurement_cmd, sizeof(read_measurement_cmd), HAL_MAX_DELAY);
    if (status != HAL_OK) {
        return status;
    }

    // Wait briefly before reading
    HAL_Delay(10);

    // Read sensor response into buffer
    status = HAL_I2C_Master_Receive(hi2c, SEN54_I2C_ADDR, buffer, len, HAL_MAX_DELAY);
    return status;
}
