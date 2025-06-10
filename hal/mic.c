#include "inc/mic.h"

uint dma_channel;
dma_channel_config dma_cfg;

/**
 * @brief Configuração do canal ADC e DMA para ler o microfone.
 */
void adc_dma_setup()
{
    adc_init();
    adc_gpio_init(MIC_PIN);
    adc_select_input(MIC_CHANNEL);
    adc_fifo_setup(true,
                   true,
                   1,
                   false,
                   false);
    adc_set_clkdiv(ADC_CLK_DIV);

    dma_channel = dma_claim_unused_channel(true);
    dma_cfg = dma_channel_get_default_config(dma_channel);

    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, true);
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);
}

/**
 * @brief Inicia a gravação do microfone.
 * @param adc_buffer Buffer onde os dados do ADC serão armazenados.
 */
void record_mic_start(uint16_t *adc_buffer)
{
    adc_fifo_drain();
    adc_run(false);

    dma_channel_configure(dma_channel,
                          &dma_cfg,
                          adc_buffer,
                          &(adc_hw->fifo),
                          SAMPLES,
                          true); // Inicia a transferência
    adc_run(true);
}

/**
 * @brief Verifica se a gravação do microfone foi concluída.
 * @return true se a gravação foi concluída, false caso contrário.
 */
bool record_mic_is_finished()
{
    return !dma_channel_is_busy(dma_channel);
}

/**
 * @brief Para a gravação do microfone e limpa o DMA.
 */
void record_mic_stop()
{
    adc_run(false);
    // Embora 'abort' não seja estritamente necessário se a transferência
    // terminou, é uma boa prática de limpeza.
    if (dma_channel_is_busy(dma_channel))
    {
        dma_channel_abort(dma_channel);
    }
}