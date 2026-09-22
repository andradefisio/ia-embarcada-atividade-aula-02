#include "mpu6050.h"

#include <stdint.h>

#include "driver/gpio.h"

#define MPU6050_ADDRESS          0x68
#define MPU6050_REG_SMPLRT_DIV   0x19
#define MPU6050_REG_CONFIG       0x1A
#define MPU6050_REG_GYRO_CONFIG  0x1B
#define MPU6050_REG_ACCEL_CONFIG 0x1C
#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_PWR_MGMT_1   0x6B
#define MPU6050_REG_WHO_AM_I     0x75

#define MPU6050_WHO_AM_I_VALUE   0x68
#define I2C_FREQUENCY_HZ         100000
#define I2C_TIMEOUT_MS           100

static esp_err_t write_register(const mpu6050_handle_t *sensor,
                                uint8_t reg,
                                uint8_t value)
{
    const uint8_t payload[] = {reg, value};
    return i2c_master_transmit(sensor->device, payload, sizeof(payload), I2C_TIMEOUT_MS);
}

static esp_err_t read_registers(const mpu6050_handle_t *sensor,
                                uint8_t start_reg,
                                uint8_t *buffer,
                                size_t length)
{
    return i2c_master_transmit_receive(sensor->device,
                                       &start_reg,
                                       sizeof(start_reg),
                                       buffer,
                                       length,
                                       I2C_TIMEOUT_MS);
}

static int16_t to_int16(uint8_t high, uint8_t low)
{
    return (int16_t)(((uint16_t)high << 8) | low);
}

esp_err_t mpu6050_init(mpu6050_handle_t *sensor, int sda_gpio, int scl_gpio)
{
    if (sensor == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    const i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = (gpio_num_t)sda_gpio,
        .scl_io_num = (gpio_num_t)scl_gpio,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t err = i2c_new_master_bus(&bus_config, &sensor->bus);
    if (err != ESP_OK) {
        return err;
    }

    const i2c_device_config_t device_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = MPU6050_ADDRESS,
        .scl_speed_hz = I2C_FREQUENCY_HZ,
    };

    err = i2c_master_bus_add_device(sensor->bus, &device_config, &sensor->device);
    if (err != ESP_OK) {
        i2c_del_master_bus(sensor->bus);
        return err;
    }

    uint8_t device_id = 0;
    err = read_registers(sensor, MPU6050_REG_WHO_AM_I, &device_id, 1);
    if (err != ESP_OK) {
        return err;
    }
    if (device_id != MPU6050_WHO_AM_I_VALUE) {
        return ESP_ERR_INVALID_RESPONSE;
    }

    // Sai do modo sleep e seleciona o oscilador interno.
    if ((err = write_register(sensor, MPU6050_REG_PWR_MGMT_1, 0x00)) != ESP_OK) {
        return err;
    }
    // Taxa de amostragem de 1 kHz / (1 + 9) = 100 Hz.
    if ((err = write_register(sensor, MPU6050_REG_SMPLRT_DIV, 0x09)) != ESP_OK) {
        return err;
    }
    // Filtro digital passa-baixas configurado em modo 3.
    if ((err = write_register(sensor, MPU6050_REG_CONFIG, 0x03)) != ESP_OK) {
        return err;
    }
    // Escalas: giroscopio +/-250 graus/s e acelerometro +/-2 g.
    if ((err = write_register(sensor, MPU6050_REG_GYRO_CONFIG, 0x00)) != ESP_OK) {
        return err;
    }
    return write_register(sensor, MPU6050_REG_ACCEL_CONFIG, 0x00);
}

esp_err_t mpu6050_read(const mpu6050_handle_t *sensor, mpu6050_data_t *data)
{
    if (sensor == NULL || data == NULL) {
        return ESP_ERR_INVALID_ARG;
    }

    uint8_t raw[14];
    esp_err_t err = read_registers(sensor, MPU6050_REG_ACCEL_XOUT_H, raw, sizeof(raw));
    if (err != ESP_OK) {
        return err;
    }

    const int16_t accel_x = to_int16(raw[0], raw[1]);
    const int16_t accel_y = to_int16(raw[2], raw[3]);
    const int16_t accel_z = to_int16(raw[4], raw[5]);
    const int16_t temperature = to_int16(raw[6], raw[7]);
    const int16_t gyro_x = to_int16(raw[8], raw[9]);
    const int16_t gyro_y = to_int16(raw[10], raw[11]);
    const int16_t gyro_z = to_int16(raw[12], raw[13]);

    data->accel_x_g = accel_x / 16384.0f;
    data->accel_y_g = accel_y / 16384.0f;
    data->accel_z_g = accel_z / 16384.0f;
    data->temperature_c = temperature / 340.0f + 36.53f;
    data->gyro_x_dps = gyro_x / 131.0f;
    data->gyro_y_dps = gyro_y / 131.0f;
    data->gyro_z_dps = gyro_z / 131.0f;

    return ESP_OK;
}
