#include "inc/oled.h"

// Variável global para armazenar as strings de texto a serem exibidas. 
char *text[] = {
    " Gravando     ",
    " Reproduzindo "};

/**
 * @brief Desenha uma linha de carregamento na tela OLED. A linha é atualizada com base no tempo decorrido desde o início da gravação. A largura da linha é proporcional ao tempo decorrido em relação à duração total da gravação.
 * @param ssd Ponteiro para o buffer do display OLED.
 * @param area Estrutura que define a área de renderização na tela.
 * @param start_time Tempo absoluto em que a gravação começou, usado para calcular o tempo decorrido.
 */
void update_loading_animation(uint8_t *ssd, struct render_area *area, absolute_time_t start_time)
{
    const float duration_s = (float)SAMPLES / SAMPLE_RATE;
    const float duration_us = duration_s * 1000000.0f;
    const int total_width = area->end_column - area->start_column;

    int64_t elapsed_us = absolute_time_diff_us(start_time, get_absolute_time());

    int current_width = (int)((elapsed_us / duration_us) * total_width);
    current_width = current_width > total_width ? total_width
                                                : (current_width < 0 ? 0 : current_width);

    ssd1306_draw_line(ssd, area->start_column, 32, area->start_column + current_width, 32, true);

    render_on_display(ssd, area);
}

/**
 * @brief Desenha o texto de gravação na tela OLED.
 * @param ssd Ponteiro para o buffer do display OLED.
 * @param area Estrutura que define a área de renderização na tela.
 */
void draw_recording_text(uint8_t *ssd, struct render_area *area)
{
    memset(ssd, 0, ssd1306_buffer_length);
    ssd1306_draw_string(ssd, 0, 5, text[0]);
    render_on_display(ssd, area);
}

/**
 * @brief Desenha a forma de onda do áudio na tela OLED.
 * @param ssd Ponteiro para o buffer do display OLED.
 * @param area Estrutura que define a área de renderização na tela.
 * @param adc_buffer Buffer contendo os dados do ADC.
 */
void draw_audio_wave(uint8_t *ssd, struct render_area *area, uint16_t *adc_buffer)
{

    const float adc_mid = 2048.0f;
    const float gain = 0.10f;

    memset(ssd, 0, ssd1306_buffer_length);

    int prev_x = 0;
    int prev_y = CENTER_Y;

    ssd1306_draw_string(ssd, 0, 5, text[1]);

    for (int i = 0; i < ssd1306_width && i < SAMPLES; i++)
    {
        int x = i;

        // Calcula a variação em torno do centro e aplica o ganho
        float delta = (float)adc_buffer[i] - adc_mid;
        int y = CENTER_Y - (int)(delta * gain);

        y = y < 0 ? 0 : (y >= ssd1306_height ? ssd1306_height - 1 : y);

        if (i > 0)
            ssd1306_draw_line(ssd, prev_x, prev_y, x, y, true);

        prev_x = x;
        prev_y = y;
        render_on_display(ssd, area);
    }
}