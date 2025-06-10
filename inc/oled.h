#ifndef OLED_H
#define OLED_H

#include "ssd1306.h"
#include "mic.h"
#include <string.h>

#define CENTER_X (ssd1306_width / 2)
#define CENTER_Y (ssd1306_height / 2)

void update_loading_animation(uint8_t *ssd, struct render_area *area, absolute_time_t start_time);
void draw_recording_text(uint8_t *ssd, struct render_area *area);
void draw_audio_wave(uint8_t *ssd, struct render_area *area, uint16_t *adc_buffer);

#endif