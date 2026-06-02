#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <stdint.h>
#include "custom_adv.h" // Required for CustomAdv_t.

// sData is declared in app.c and used for BLE advertising.
extern CustomAdv_t sData;
// Advertising handle.
extern uint8_t advertising_set_handle;

/**
 * @brief Initialize the LCD module with DMD and GLIB.
 */
void lcd_init_and_clear(void);

/**
 * @brief Update the LCD screen with new sensor data.
 */
void lcd_update(float temp, float hum, uint32_t sensor_period_ms);

#endif // LCD_DISPLAY_H
