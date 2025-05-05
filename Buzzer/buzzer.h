/*
 * buzzer.h
 *
 *  Created on: May 5, 2025
 *      Author: cldlv
 */

#ifndef BUZZER_H
#define BUZZER_H

#include "stm32l4xx_hal.h"

void buzzer_on(void);
void buzzer_off(void);
void buzzer_beep(uint32_t duration_ms);

#endif
