#include "inc/buzzer.h"

uint slice_num;
uint channel_num;

/**
 * @brief Inicializa o buzzer configurando o pino como PWM.
 */
void pwm_buzzer_init()
{
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);

    slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    channel_num = pwm_gpio_to_channel(BUZZER_PIN);

    pwm_config config = pwm_get_default_config();

    pwm_config_set_wrap(&config, 4095);     
    pwm_config_set_clkdiv(&config, 1.0f); 
    pwm_init(slice_num, &config, true);
    pwm_set_gpio_level(BUZZER_PIN, 0);
}

/**
 * @brief Reproduz o som do buzzer com base nos dados do ADC.
 * 
 * @param adc_buffer Buffer contendo os dados do ADC.
 */
void buzzer_play(uint16_t *adc_buffer)
{
    const uint32_t delay_us = 1000000 / SAMPLE_RATE;
    const uint16_t center = 2048; 
    const float gain = 4.0f;

    for (size_t i = 0; i < SAMPLES; i++)
    {
        if (i % 100 == 0) printf("%d ", adc_buffer[i]);

        int32_t sample = adc_buffer[i] - center;
        sample = (int32_t)(sample * gain) + center;
        sample = sample < 0 ? 0 : sample > 4095 ? 4095 : sample;

        //adc_buffer[i] = sample;

        pwm_set_gpio_level(BUZZER_PIN, /* adc_buffer[i] */ sample);
        sleep_us(delay_us);
    }

    pwm_set_gpio_level(BUZZER_PIN, 0);
}