#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <stdint.h>
#include "custom_adv.h" // <-- THÊM DÒNG NÀY ĐỂ SỬA LỖI unknown type name

// Khai báo biến sData từ app.c (cần cho BLE)
extern CustomAdv_t sData;
// Khai báo handle quảng bá
extern uint8_t advertising_set_handle;

/**
 * @brief Khởi tạo mô-đun LCD (DMD, GLIB).
 */
void lcd_init_and_clear(void);

/**
 * @brief Cập nhật màn hình LCD với dữ liệu mới.
 */
void lcd_update(float temp, float hum, uint32_t sensor_period_ms);

#endif // LCD_DISPLAY_H
