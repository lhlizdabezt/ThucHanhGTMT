#ifndef AHT20_H
#define AHT20_H

#include "sl_status.h"
#include "sl_i2cspm.h"
#include <stdbool.h>

// I2C address for the AHT20 sensor.
#define AHT20_I2C_ADDR 0x38

/**
 * @brief Initialize the AHT20 sensor by sending the calibration command.
 */
sl_status_t aht20_init(sl_i2cspm_t *i2cspm_handle);

/**
 * @brief Read temperature and humidity from the AHT20 sensor.
 */
sl_status_t aht20_read(sl_i2cspm_t *i2cspm_handle, float *temp, float *hum);

#endif // AHT20_H
