#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_log.h"

#include "mpu6050.h"

#define I2C_SDA_GPIO 8
#define I2C_SCL_GPIO 9

static const char *TAG = "MPU6050";

void app_main(void)
{
    mpu6050_handle_t sensor = {0};

    ESP_LOGI(TAG, "Iniciando MPU6050 em SDA=GPIO%d e SCL=GPIO%d", I2C_SDA_GPIO, I2C_SCL_GPIO);
    ESP_ERROR_CHECK(mpu6050_init(&sensor, I2C_SDA_GPIO, I2C_SCL_GPIO));
    ESP_LOGI(TAG, "MPU6050 identificado no endereco I2C 0x68");

    while (true) {
        mpu6050_data_t data;
        esp_err_t err = mpu6050_read(&sensor, &data);

        if (err == ESP_OK) {
            ESP_LOGI(TAG,
                     "Acel [g] X=%+.3f Y=%+.3f Z=%+.3f | "
                     "Giro [graus/s] X=%+.2f Y=%+.2f Z=%+.2f | Temp=%.2f C",
                     data.accel_x_g,
                     data.accel_y_g,
                     data.accel_z_g,
                     data.gyro_x_dps,
                     data.gyro_y_dps,
                     data.gyro_z_dps,
                     data.temperature_c);
        } else {
            ESP_LOGE(TAG, "Falha na leitura do sensor: %s", esp_err_to_name(err));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
