# Projetos de Sistemas Embarcados - EmbarcaTech 2025

- Autor: [Guilherme Westphall](https://github.com/west7)
- Curso: Residência Tecnológica em Sistemas Embarcados
- Instituição: EmbarcaTech - HBr

  📍 Brasília, Junho de 2025

---

# Sintesizador de áudio

> Projetar um gravador e reprodutor de áudio com a Raspberry Pi Pico W, utilizando um microfone com amplificador de áudio (MAX4466EXK) e dois Buzzers para reprodução de áudio. 

## Objetivo 🎯

O objetivo deste projeto é desenvolver um gravador e reprodutor de áudio utilizando a Raspberry Pi Pico W, um microfone com amplificador de áudio (MAX4466EXK) e dois buzzers para reprodução de áudio. O projeto visa explorar as capacidades de captura e reprodução de som da placa, além de práticas com PWM, ADC, I2C e concorrência de tarefas.

## Componentes Utilizados 🛠️

| Componente           | Descrição                       | Pinagem       |
| -------------------- | ------------------------------- | ------------- |
| Raspberry Pi Pico W  | Integrada na BitDogLab          | -             |
| Microfone MAX4466EXK | Microfone com amplificador      | GPIO28        |
| Buzzer A             | Buzzer para reprodução de áudio | GPIO21        |
| Botão A              | Botão para iniciar gravação     | GPIO5         |
| Botão B              | Buzzer para reprodução de áudio | GPIO6         |
| Display OLED         | Display para visualização       | GPIO14 (SDA) GPIO15 (SCL) |
| LED RGB | LED RGB para indicação de status | GPIO13 (R) GPIO12 (B) GPIO11 (G) |


## Estrutura do Projeto 

```
├── hal                      # Hardware Abstraction Layer
|   ├── buzzer.c             # Funções para controle e inicialização do buzzer
|   ├── mic.c                # Funções para controle e inicialização do microfone
|   ├── oled.c               # Funções para controle e inicialização do display OLED
|   └──  ssd1306.c           # Funções para controle e inicialização do display SSD1306
├── inc                      # Arquivos de cabeçalho
|   ├── buzzer.h             # Definições e protótipos para buzzer
|   ├── mic.h                # Definições e protótipos para microfone
|   ├── oled.h               # Definições e protótipos para display OLED
|   ├── ssd1306_font.h       # Arquivos para uso do SSD1306
|   ├── ssd1306_i2c.h        # Arquivo para uso do SSD1306
|   └── ssd1306.h            # Arquivo para uso do SSD1306
|
├── CMakeLists.txt           # Arquivo de configuração do CMake
├── main.c                   # Função principal do programa
├── README.md                # Documentação do projeto
├── LICENSE                  # Licença do projeto
├── pico_sdk_import.cmake    # Importação do SDK do Raspberry Pi Pico
└── adc_to_wav.py            # Conversão de ADC para WAV para debug
```
     
## Reflexões 💭

### Quais técnicas de programação podemos usar para melhorar a gravação e a reprodução do áudio?

- Uso de *ring buffers*, ou buffers circulares, para armazenar os dados de áudio, permitindo uma leitura e escrita mais eficiente.
- Implementação de filtros digitais para melhorar a qualidade do áudio, como filtros passa-baixa ou passa-alta.
- Um que já está sendo aplicado é a utilização de **DMA (Direct Memory Access)** para transferir os dados do ADC para a memória, liberando a CPU para outras tarefas, por isso, por exemplo, que é possível executar a animação do display OLED enquanto o áudio é gravado.

### Como gravar áudios mais extensos sem prejudicar a qualidade?

- Compressão de áudio, algoritmos simples como ADPCM permitem comprimir o áudio para ocupar menos espaço com pouca perda de qualidade.
- Armazenamento externo, utilizando um cartão SD ou outro meio de armazenamento externo para gravar áudios mais longos sem se preocupar com o espaço limitado da memória interna da Raspberry Pi Pico W.
- Redução da taxa de amostragem, dependendo do tipo de áudio, uma taxa de 8kHz pode ser suficiente para voz, por exemplo.
- Usar o ADC em 12 ou 8 bits ao invés de 16 bits, reduzindo a resolução do áudio proporcionalmente, mas também o tamanho do arquivo.

## Vídeo 📹

<center>

[![Vídeo do Projeto](https://img.youtube.com/vi/tMY6fcm6D1A/hqdefault.jpg)](https://youtube.com/shorts/tMY6fcm6D1A?feature=share)

</center>

## Execução 🧪

1. Faça o clone do projeto:

```bash
git clone https://github.com/EmbarcaTech-2025/projeto-orientado-2-sintetizador-de-udio-west7.git
```

2. Compile e Embarque o firmware na Raspberry:

```bash
mkdir build
cd build
cmake -G Ninja ..
ninja
cp sintetizador_audio.uf2 /media/$USER/RPI-RP2/
```

