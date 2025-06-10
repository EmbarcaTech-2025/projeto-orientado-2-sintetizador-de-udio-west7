#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/clocks.h"
#include "hardware/pwm.h"
#include "inc/mic.h"

#define BUZZER_PIN 21

void pwm_buzzer_init();
void buzzer_play(uint16_t *adc_buffer);

#endif 