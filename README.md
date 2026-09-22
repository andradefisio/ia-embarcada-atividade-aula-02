# Atividade Avaliativa Pratica 2/6 - Leitura de sensor

Aplicacao em C para ESP32-S3 que inicializa um MPU6050 por I2C, le aceleracao,
giroscopio e temperatura e imprime as medicoes no monitor serial. O circuito e
simulado no Wokwi e o firmware e compilado com ESP-IDF.

Build validado em 21/09/2026 com ESP-IDF v5.5.5 para o alvo `esp32s3`.

Repositorio da entrega:
<https://github.com/andradefisio/ia-embarcada-atividade-aula-02>

## Componentes e ligacoes

| ESP32-S3 DevKitC-1 | MPU6050 | Funcao |
|---|---|---|
| 3V3 | VCC | Alimentacao |
| GND | GND | Terra |
| GPIO 8 | SDA | Dados I2C |
| GPIO 9 | SCL | Clock I2C |

O pino AD0 permanece desconectado, portanto o endereco I2C padrao e `0x68`.

O arquivo `diagram.json` ja contem essa montagem.

## Configuracao do ambiente

1. Instale o **ESP-IDF Installation Manager** e uma versao estavel do ESP-IDF.
2. No VS Code, instale as extensoes **ESP-IDF** (Espressif) e **Wokwi Simulator**.
3. Execute `ESP-IDF: Configure ESP-IDF Extension` pela paleta de comandos.
4. Abra esta pasta no VS Code e selecione o alvo `esp32s3`.
5. Entre em sua conta no Wokwi quando a extensao solicitar. A licenca gratuita
   ou trial precisa estar ativa para executar a simulacao integrada.

## Compilacao

No terminal configurado do ESP-IDF:

```text
idf.py set-target esp32s3
idf.py build
```

O `wokwi.toml` aponta para estes artefatos gerados:

- `build/leitura_mpu6050.bin`
- `build/leitura_mpu6050.elf`

## Simulacao e monitor serial

1. Compile o projeto sem erros.
2. Abra `diagram.json` para conferir visualmente o circuito.
3. Pressione `F1` e execute `Wokwi: Start Simulator`.
4. No MPU6050, altere os controles de aceleracao/rotacao para variar os dados.
5. Capture uma unica tela ampla ou telas separadas mostrando:
   - o circuito completo;
   - a mensagem de build concluido;
   - o monitor serial com varias leituras;
   - as extensoes ESP-IDF e Wokwi configuradas.

Saida serial esperada:

```text
I (...) MPU6050: MPU6050 identificado no endereco I2C 0x68
I (...) MPU6050: Acel [g] X=+0.000 Y=+0.000 Z=+1.000 | Giro [graus/s] X=+0.00 Y=+0.00 Z=+0.00 | Temp=24.00 C
```

## Estrutura

```text
.
|-- CMakeLists.txt
|-- diagram.json
|-- sdkconfig.defaults
|-- wokwi.toml
|-- main/
|   |-- CMakeLists.txt
|   |-- main.c
|   |-- mpu6050.c
|   `-- mpu6050.h
`-- evidencias/
```

## Entrega

Antes de enviar:

- substitua o nome do autor em `diagram.json`, se desejar;
- coloque os screenshots finais em `evidencias/` e versione-os;
- entregue o link do repositorio e o screenshot solicitado.

Nao versione tokens, senhas, arquivos de configuracao pessoal ou credenciais do
Wokwi/GitHub.
