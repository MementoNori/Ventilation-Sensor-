#include "buzzer.h"

void buzzer_on(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
}

void buzzer_off(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
}

void buzzer_beep(uint32_t duration_ms) {
    buzzer_on();
    HAL_Delay(duration_ms);
    buzzer_off();
}
