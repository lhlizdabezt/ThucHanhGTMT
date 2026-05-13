#ifndef AHT20_H
#define AHT20_H

#include "sl_status.h"
#include "sl_i2cspm.h"
#include <stdbool.h>

// Địa chỉ I2C của AHT20
#define AHT20_I2C_ADDR 0x38

/**
 * @brief Khởi tạo cảm biến AHT20 (gửi lệnh Calibrate).
 */
sl_status_t aht20_init(sl_i2cspm_t *i2cspm_handle);

/**
 * @brief Đọc nhiệt độ và độ ẩm từ AHT20.
 */
sl_status_t aht20_read(sl_i2cspm_t *i2cspm_handle, float *temp, float *hum);

#endif // AHT20_H
