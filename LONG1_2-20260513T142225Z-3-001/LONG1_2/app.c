/***************************************************************************//**
 * @file
 * @brief App: AHT20 + LCD + BLE advertising + UART log without using %f.
 ******************************************************************************/

#include "em_common.h"
#include "app_assert.h"
#include "sl_bluetooth.h"
#include "gatt_db.h"
#include "app_log.h"
#include "app_timer.h"

#include "sl_i2cspm_instances.h"
#include "aht20.h"
#include "lcd_display.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE_NAME   "F3"
#define I2C_HANDLE    sl_i2cspm_i2c0

// ====================== GLOBAL VARIABLES ======================

static float current_temp = 0.0f;
static float current_hum  = 0.0f;

static app_timer_t sensor_timer;
static uint32_t    sensor_period_ms = 1000;

static uint8_t     adv_handle = 0xFF;

// ====================== BCD HELPER ========================

static uint8_t dec_to_bcd(uint8_t val)
{
  return (uint8_t)(((val / 10) << 4) | (val % 10));
}

// ================== BLE ADVERTISING PAYLOAD UPDATE ==================

static void update_ble_payload(float temp, float hum)
{
  uint8_t adv_data[31];
  uint8_t len = 0;

  // ---- FLAGS ----
  adv_data[len++] = 2;       // length
  adv_data[len++] = 0x01;    // Flags
  adv_data[len++] = 0x06;    // General discoverable + BR/EDR not supported

  // ---- DEVICE NAME ----
  const char *name = DEVICE_NAME;
  uint8_t name_len = (uint8_t)strlen(name);
  if (name_len > 29) name_len = 29;

  adv_data[len++] = 1 + name_len;   // length (type + name)
  adv_data[len++] = 0x09;           // Complete Local Name
  memcpy(&adv_data[len], name, name_len);
  len += name_len;

  // ---- SENSOR DATA -> 4 BYTE BCD ----
  int t_int = (int)temp;
  int t_dec = (int)((temp - t_int) * 10.0f);
  if (t_dec < 0) t_dec = -t_dec;

  int h_int = (int)hum;
  int h_dec = (int)((hum - h_int) * 10.0f);
  if (h_dec < 0) h_dec = -h_dec;

  uint8_t t_int_bcd = dec_to_bcd((uint8_t)t_int);
  uint8_t t_dec_bcd = dec_to_bcd((uint8_t)t_dec);
  uint8_t h_int_bcd = dec_to_bcd((uint8_t)h_int);
  uint8_t h_dec_bcd = dec_to_bcd((uint8_t)h_dec);

  // Manufacturer Specific Data
  adv_data[len++] = 1 + 2 + 4; // length = type + 2 byte company ID + 4 data bytes
  adv_data[len++] = 0xFF;      // Manufacturer Specific
  adv_data[len++] = 0xFF;      // Company ID LSB (demo)
  adv_data[len++] = 0x02;      // Company ID MSB (demo)
  adv_data[len++] = t_int_bcd;
  adv_data[len++] = t_dec_bcd;
  adv_data[len++] = h_int_bcd;
  adv_data[len++] = h_dec_bcd;

  // Send data to the Bluetooth stack.
  sl_bt_legacy_advertiser_set_data(
      adv_handle,
      sl_bt_advertiser_advertising_data_packet,
      len,
      adv_data);
}

// ================== SENSOR READ TIMER =====================

static void sensor_timer_cb(app_timer_t *timer, void *data)
{
  (void)timer;
  (void)data;

  if (aht20_read(I2C_HANDLE, &current_temp, &current_hum) == SL_STATUS_OK) {
    // Update LCD.
      lcd_update(current_temp, current_hum, sensor_period_ms);

    // Update BLE advertising data.
    update_ble_payload(current_temp, current_hum);

    // Print UART data without %f support.
    // Keep one decimal digit by multiplying by 10.
    int16_t temp10 = (int16_t)(current_temp * 10.0f);
    int16_t hum10  = (int16_t)(current_hum  * 10.0f);

    int t_int  = temp10 / 10;
    int t_frac = abs(temp10 % 10);

    int h_int  = hum10 / 10;
    int h_frac = abs(hum10 % 10);

    app_log("D:%d.%d,%d.%d\r\n",
            t_int, t_frac,
            h_int, h_frac);
  } else {
    app_log("AHT20 read FAIL\r\n");
  }
}

// ================== BLUETOOTH EVENTS =======================

void sl_bt_on_event(sl_bt_msg_t *evt)
{
  sl_status_t sc;
  uint32_t evt_id = SL_BT_MSG_ID(evt->header);

  switch (evt_id) {
    case sl_bt_evt_system_boot_id:
      app_log("BLE boot\r\n");

      // Create advertising set.
      sc = sl_bt_advertiser_create_set(&adv_handle);
      app_assert_status(sc);

      // Advertising period is about 50 ms (80 * 0.625 ms).
      sc = sl_bt_advertiser_set_timing(adv_handle,
                                       80, 80,
                                       0, 0);
      app_assert_status(sc);

      // Advertise on channels 37, 38 and 39.
      sc = sl_bt_advertiser_set_channel_map(adv_handle, 7);
      app_assert_status(sc);

      // Initial payload with placeholder values, updated later.
      update_ble_payload(25.3f, 64.7f);

      // Start non-connectable advertising.
      sc = sl_bt_legacy_advertiser_start(adv_handle,
                                         sl_bt_advertiser_non_connectable);
      app_assert_status(sc);

      app_log("Started advertising\r\n");
      break;

    default:
      break;
  }
}

// ================== APP INIT & PROCESS =====================

void app_init(void)
{
  lcd_init_and_clear();

  if (aht20_init(I2C_HANDLE) == SL_STATUS_OK) {
    app_log("AHT20 OK\r\n");
  } else {
    app_log("AHT20 FAIL\r\n");
  }

  // Start the sensor read timer.
  app_timer_start(&sensor_timer,
                  sensor_period_ms,
                  sensor_timer_cb,
                  NULL,
                  true);
}

void app_process_action(void)
{
  // No extra processing is required in the main loop.
}
