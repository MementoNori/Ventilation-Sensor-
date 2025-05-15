/*
 * buzzer_music.h
 *
 *  Created on: May 12, 2025
 *      Author: cldlvw
 */

#ifndef INC_BUZZER_MUSIC_H_
#define INC_BUZZER_MUSIC_H_


#include "stm32l4xx_hal.h"

void play_melody(void);
void play_note(char note, uint32_t duration_ms);
void HAL_Delay_us(uint32_t us);

#endif /* INC_BUZZER_MUSIC_H_ */

/*
 * PLEASE INCLUDE: #include "buzzer_music.h"
 * === in main function ===
 *
 *
 *
 * == for twinkle twinkle little star:
 *    int length = 15;
      char notes[] = "ccggaagffeeddc ";
      int beats[]  = {1,1,1,1,1,1,2,1,1,1,1,1,1,2,4};
      int tempo = 300;

  == for TOM THE TANK ENGINE
  	  int length = 8;
  	  char notes[] = "gahCDEG "
  	  int beats[] = {1,1,1,2,1,1,4};
  	  int tempo = 300;

      while (1) {
          for (int i = 0; i < length; i++) {
              if (notes[i] == ' ') {
                  HAL_Delay(beats[i] * tempo);
              } else {
                  play_note(notes[i], beats[i] * tempo);
              }
              HAL_Delay(tempo / 2);
          }
      }
 *
 * */
