/*
 * buzzer_music.c
 *
 *  Created on: May 12, 2025
 *      Author: cldlv
 */

#include "buzzer_music.h"
#include "buzzer.h"         // damit du buzzer_on/off benutzen kannst
//#include "stm32f1xx_hal.h"  // oder je nach deiner MCU-Serie


void HAL_Delay_us(uint32_t us)
{
    // HCLK ist 4 MHz, also gibt es 4 Taktzyklen pro Mikrosekunde
    uint32_t delay_cycles = 4;  // Taktzyklen pro Mikrosekunde bei 4 MHz

    volatile uint32_t delay_count;
    for (uint32_t i = 0; i < us; i++) {
        // Eine Schleife für jede Mikrosekunde
        for (delay_count = 0; delay_count < delay_cycles; delay_count++) {
            // Ein leerer Loop als Verzögerung
        }
    }
}

static const char notes[] = "ccggaagffeeddc ";
static const uint32_t beats[] = { 1,1,1,1,1,1,2,1,1,1,1,1,1,2,4 };
static const uint32_t tempo = 300;

void play_note(char note, uint32_t duration_ms) {
    // Notennamen mit zugehörigen Tonhöhen
	const char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C', 'D', 'E', 'G' }; // Hinweis: G = gis (G♯)
	const uint16_t tones[] = {
	    1915, // c (261.63 Hz)
	    1700, // d (293.66 Hz)
	    1519, // e (329.63 Hz)
	    1432, // f (349.23 Hz)
	    1275, // g (392.00 Hz)
	    1136, // a (440.00 Hz)
	    1014, // b (493.88 Hz)
	    956,  // C (523.25 Hz)
	    851,  // D (587.33 Hz)
	    758,  // E (659.26 Hz)
		1204  // G = gis (415.30 Hz)
	};



    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            uint16_t tone = tones[i];
            uint32_t cycles = (duration_ms * 1000L) / (tone * 2);
            for (uint32_t j = 0; j < cycles; j++) {
                buzzer_on();
                HAL_Delay_us(tone);   // eigene Mikrosekunden-Funktion nötig!
                buzzer_off();
                HAL_Delay_us(tone);
            }
            return;
        }
    }
}

void play_melody(void) {
    for (int i = 0; i < sizeof(beats)/sizeof(beats[0]); i++) {
        if (notes[i] == ' ') {
            HAL_Delay(beats[i] * tempo);
        } else {
            play_note(notes[i], beats[i] * tempo);
        }
        HAL_Delay(tempo / 2);
    }
}

