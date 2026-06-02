#ifndef _CUSTOM_ADV_H_
#define _CUSTOM_ADV_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sl_bt_api.h"
#include "app_assert.h"
#include "app_log.h"

#define NAME_MAX_LENGTH 14
#define FLAG       0x06
#define COMPANY_ID 0x02FF

typedef struct __attribute__((packed))
{
  // --- Part 1: Flags ---
  uint8_t len_flags;
  uint8_t type_flags;
  uint8_t val_flags;

  // --- Part 2: Manufacturer data ---
  uint8_t len_manuf;
  uint8_t type_manuf;
  uint8_t company_LO;
  uint8_t company_HI;

  // --- Temperature and humidity data in BCD format ---
  // Four bytes preserve integer and decimal parts.
  uint8_t temp_int; // Temperature integer part, for example 30.
  uint8_t temp_dec; // Temperature decimal part, for example 5.
  uint8_t hum_int;  // Humidity integer part, for example 58.
  uint8_t hum_dec;  // Humidity decimal part, for example 2.

  // --- Part 3: Device name ---
  uint8_t len_name;
  uint8_t type_name;
  char name[NAME_MAX_LENGTH];

  char dummy;
  uint8_t data_size;
} CustomAdv_t;

// Initialize the advertising packet.
void fill_adv_packet(CustomAdv_t *pData, uint8_t *pDataSize, uint8_t flags, uint16_t companyID,
                     char *name);

void start_adv(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle);

// Update the packet with four BCD values.
void update_adv_data(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle,
                     uint8_t t_int, uint8_t t_dec, uint8_t h_int, uint8_t h_dec);

#ifdef __cplusplus
}
#endif

#endif // _CUSTOM_ADV_H_
