# Atividade Avaliativa Prática 2/6 — Leitura de sensor

Neste trabalho desenvolvi uma aplicação embarcada em C para ler os dados de um
sensor MPU6050 utilizando uma placa ESP32-S3. O projeto foi compilado com o
ESP-IDF no VS Code e executado no simulador Wokwi.

O programa lê os valores de aceleração, rotação e temperatura do sensor e
mostra uma nova medição no monitor serial a cada segundo.

Repositório da atividade:
<https://github.com/andradefisio/ia-embarcada-atividade-aula-02>

## Componentes utilizados

- ESP32-S3 DevKitC-1;
- sensor MPU6050;
- extensão ESP-IDF para o VS Code;
- extensão Wokwi Simulator;
- ESP-IDF v5.5.5.

## Ligações do circuito

| ESP32-S3 | MPU6050 | Função |
|---|---|---|
| 3V3 | VCC | Alimentação de 3,3 V |
| GND | GND | Terra |
| GPIO 8 | SDA | Dados do barramento I2C |
| GPIO 9 | SCL | Clock do barramento I2C |

O pino AD0 ficou desconectado. Dessa forma, o endereço I2C utilizado pelo
MPU6050 é `0x68`. A montagem completa está no arquivo `diagram.json`.

## Funcionamento do programa

Na inicialização, o programa configura o barramento I2C, verifica o registrador
`WHO_AM_I` e retira o MPU6050 do modo de repouso. Em seguida, configura as
escalas do acelerômetro e do giroscópio.

Depois da inicialização, são lidos 14 bytes a partir do registrador de
aceleração. Os valores brutos são convertidos para:

- aceleração em `g`;
- velocidade angular em graus por segundo;
- temperatura em graus Celsius.

As leituras são enviadas pela UART0, em 115200 baud, para o monitor serial do
Wokwi. O intervalo entre as leituras é de 1 segundo.

## Compilação

No VS Code, abri diretamente a pasta `atividade-pratica-2-leitura-sensor` e
executei a tarefa de compilação com `Ctrl+Shift+B`:

```text
ESP-IDF: Build (ESP32-S3)
```

Também é possível compilar pelo terminal configurado do ESP-IDF:

```text
idf.py set-target esp32s3
idf.py build
```

A compilação foi validada em 22/09/2026 com o ESP-IDF v5.5.5. O processo foi
concluído sem erros e gerou os arquivos:

- `build/leitura_mpu6050.bin`;
- `build/leitura_mpu6050.elf`.

## Execução no Wokwi

Depois da compilação, iniciei a simulação pela paleta de comandos do VS Code:

```text
Wokwi: Start Simulator
```

Durante a simulação, alterei os valores de aceleração, rotação e temperatura nos
controles do MPU6050. As mudanças apareceram corretamente no monitor serial.

Exemplo de leitura:

```text
I (...) MPU6050: Acel [g] X=+0.500 Y=+0.600 Z=+0.750 | Giro [graus/s] X=+70.00 Y=+90.00 Z=+100.00 | Temp=48.00 C
```

## Evidências

- [Extensões ESP-IDF e Wokwi instaladas](evidencias/01-extensoes-esp-idf-wokwi.png);
- [circuito montado no Wokwi](evidencias/02-circuito-wokwi.png);
- [compilação concluída sem erros](evidencias/03-build-sem-erros.png);
- [monitor serial mostrando as leituras](evidencias/04-monitor-serial.png).

## Estrutura do projeto

```text
.
|-- .vscode/
|   `-- tasks.json
|-- evidencias/
|-- main/
|   |-- CMakeLists.txt
|   |-- main.c
|   |-- mpu6050.c
|   `-- mpu6050.h
|-- CMakeLists.txt
|-- diagram.json
|-- sdkconfig.defaults
`-- wokwi.toml
```
