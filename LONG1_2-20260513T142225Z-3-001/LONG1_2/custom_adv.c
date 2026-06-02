#include <string.h>
#include "custom_adv.h"
#include "stdio.h"
#include "app_assert.h"
#include "app_log.h"

void fill_adv_packet(CustomAdv_t *pData, uint8_t *pDataSize, uint8_t flags, uint16_t companyID,
                     char *name)
{
  int n;

  // 1. Flags
  pData->len_flags = 0x02;
  pData->type_flags = 0x01;
  pData->val_flags = flags;

  // 2. Manufacturer Data
  // Len = 1(type) + 2(company) + 4(data: T_int, T_dec, H_int, H_dec) = 7 bytes
  pData->len_manuf = 7;
  pData->type_manuf = 0xFF;
  pData->company_LO = companyID & 0xFF;
  pData->company_HI = (companyID >> 8) & 0xFF;

  // Initialize values to zero.
  pData->temp_int = 0;
  pData->temp_dec = 0;
  pData->hum_int = 0;
  pData->hum_dec = 0;

  // 3. Local Name
  n = strlen(name);
  if (n > NAME_MAX_LENGTH) n = NAME_MAX_LENGTH;

  pData->type_name = 0x09; // Complete Local Name
  strncpy(pData->name, name, n);
  pData->len_name = 1 + n;

  // Calculate total payload size.
  *pDataSize = 3 + (1 + pData->len_manuf) + (1 + pData->len_name);
}

void start_adv(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle)
{
  sl_status_t sc;
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, data_size, (const uint8_t *)pData);
  app_assert_status(sc);
  sc = sl_bt_legacy_advertiser_start(advertising_set_handle, sl_bt_legacy_advertiser_connectable);
  app_assert_status(sc);
}

// Update four BCD values.
void update_adv_data(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle,
                     uint8_t t_int, uint8_t t_dec, uint8_t h_int, uint8_t h_dec)
{
  sl_status_t sc;

  // Update BCD data in the packet.
  pData->temp_int = t_int;
  pData->temp_dec = t_dec;
  pData->hum_int = h_int;
  pData->hum_dec = h_dec;

  // Send the updated packet.
  sc = sl_bt_legacy_advertiser_set_data(advertising_set_handle, 0, data_size, (const uint8_t *)pData);

  if (sc != SL_STATUS_OK) {
    // app_log("Update failed\r\n");
  }
}
