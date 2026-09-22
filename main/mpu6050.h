#ifndef MPU6050_H
#define MPU6050_H

#include "driver/i2c_master.h"
#include "esp_err.h"

typedef struct {
    float accel_x_g;
    float accel_y_g;
    float accel_z_g;
    float gyro_x_dps;
    float gyro_y_dps;
    float gyro_z_dps;
    float temperature_c;
} mpu6050_data_t;

typedef struct {
    i2c_master_bus_handle_t bus;
    i2c_master_dev_handle_t device;
} mpu6050_handle_t;

esp_err_t mpu6050_init(mpu6050_handle_t *sensor, int sda_gpio, int scl_gpio);
esp_err_t mpu6050_read(const mpu6050_handle_t *sensor, mpu6050_data_t *data);

#endif
