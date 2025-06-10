#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "inc/mic.h"
#include "inc/buzzer.h"
#include "inc/ssd1306.h"
#include "inc/oled.h"

#define BUTTON_A 5
#define BUTTON_B 6
#define G_LED 11
#define B_LED 12
#define R_LED 13

#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1

// Variáveis globais para o estado dos botões
volatile bool last_b_state = true;
volatile bool last_a_state = true;

// Estado do sistema
typedef enum
{
    IDLE,
    RECORDING,
    PROCESSING,
    PLAYING
} state_t;

state_t state = IDLE;

struct render_area frame = {
    .start_column = 0,
    .end_column = ssd1306_width - 1,
    .start_page = 0,
    .end_page = ssd1306_n_pages - 1,
};

int main()
{
    stdio_init_all();

    // Configuração do Botão A
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Configuração do Botão B
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configuração dos LEDs RGB
    gpio_init(G_LED);
    gpio_set_dir(G_LED, GPIO_OUT);
    gpio_init(B_LED);
    gpio_set_dir(B_LED, GPIO_OUT);
    gpio_init(R_LED);
    gpio_set_dir(R_LED, GPIO_OUT);

    // Configuração do I2C para o OLED SSD1306
    i2c_init(I2C_PORT, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    ssd1306_init();

    calculate_render_area_buffer_length(&frame);
    uint8_t ssd[ssd1306_buffer_length];
    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame);

    adc_dma_setup();
    sleep_ms(100);
    pwm_buzzer_init();
    sleep_ms(100);

    uint16_t adc_buffer[SAMPLES];
    absolute_time_t recording_start_time;
    bool is_recording_active = false;
    
    gpio_put(B_LED, 1);

    while (true)
    {
        bool current_a_state = gpio_get(BUTTON_A);
        bool current_b_state = gpio_get(BUTTON_B);

        switch (state)
        {
        // Estado ocioso, aguarda o botão A ser pressionado
        case IDLE:
            if (!current_a_state && last_a_state)
            {
                state = RECORDING;
                is_recording_active = false;
                gpio_put(B_LED, 0);
            }
            break;
        // Estado de gravação, aguarda o buffer ser preenchido através do DMA e renderiza a animação de carregamento
        case RECORDING:
            if (!is_recording_active)
            {
                gpio_put(R_LED, 1);

                draw_recording_text(ssd, &frame);

                recording_start_time = get_absolute_time();
                record_mic_start(adc_buffer);

                is_recording_active = true;
            }

            update_loading_animation(ssd, &frame, recording_start_time);

            if (record_mic_is_finished())
            {
                record_mic_stop();
                gpio_put(R_LED, 0);
                is_recording_active = false;
                state = PROCESSING;
            }
            break;
        
        // Estado de processamento, aguarda o botão B ser pressionado para tocar o áudio gravado ou o botão A para voltar ao estado de gravação
        case PROCESSING:
            memset(ssd, 0, ssd1306_buffer_length);
            render_on_display(ssd, &frame);
            if ((time_us_32() / 500000) % 2)
                gpio_put(B_LED, 1);
            else
                gpio_put(B_LED, 0);

            if (!current_b_state && last_b_state)
            {
                state = PLAYING;
                gpio_put(B_LED, 0);
            }
            else if (!current_a_state && last_a_state)
            {
                state = RECORDING;
                gpio_put(B_LED, 0);
            }
            break;
        // Estado de reprodução, toca o áudio gravado e depois renderiza a forma de onda no OLED
        case PLAYING:
            gpio_put(G_LED, 1);
            buzzer_play(adc_buffer);
            draw_audio_wave(ssd, &frame, adc_buffer);
            gpio_put(G_LED, 0);
            
            state = PROCESSING;
            break;
        }
        // Atualiza o estado dos botões
        last_a_state = current_a_state;
        last_b_state = current_b_state;
        sleep_ms(50);
    }
}


