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
  // --- Phần 1: Cờ (Flags) ---
  uint8_t len_flags;
  uint8_t type_flags;
  uint8_t val_flags;

  // --- Phần 2: Dữ liệu nhà sản xuất (Manufacturer Data) ---
  uint8_t len_manuf;
  uint8_t type_manuf;
  uint8_t company_LO;
  uint8_t company_HI;

  // --- SỬA ĐỔI: Dữ liệu Nhiệt độ & Độ ẩm (Dạng BCD) ---
  // 4 byte để hiển thị rõ phần nguyên và thập phân
  uint8_t temp_int; // Nhiệt độ phần nguyên (VD: 30)
  uint8_t temp_dec; // Nhiệt độ phần lẻ (VD: 5)
  uint8_t hum_int;  // Độ ẩm phần nguyên (VD: 58)
  uint8_t hum_dec;  // Độ ẩm phần lẻ (VD: 2)

  // --- Phần 3: Tên thiết bị ---
  uint8_t len_name;
  uint8_t type_name;
  char name[NAME_MAX_LENGTH];

  char dummy;
  uint8_t data_size;
} CustomAdv_t;

// Hàm khởi tạo
void fill_adv_packet(CustomAdv_t *pData, uint8_t *pDataSize, uint8_t flags, uint16_t companyID,
                     char *name);

void start_adv(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle);

// Hàm cập nhật nhận 4 giá trị BCD
void update_adv_data(CustomAdv_t *pData, uint8_t data_size, uint8_t advertising_set_handle,
                     uint8_t t_int, uint8_t t_dec, uint8_t h_int, uint8_t h_dec);

#ifdef __cplusplus
}
#endif

#endif // _CUSTOM_ADV_H_
