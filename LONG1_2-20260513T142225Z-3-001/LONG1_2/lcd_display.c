#include "lcd_display.h"
#include "glib.h"
#include "dmd.h"
#include <stdio.h>

// Biến GLIB toàn cục
static GLIB_Context_t glibContext;

/**
 * @brief Khởi tạo mô-đun LCD (DMD, GLIB).
 */
void lcd_init_and_clear(void)
{
  DMD_init(0);
  GLIB_contextInit(&glibContext);
  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;

  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8); // Dùng font nhỏ
  GLIB_clear(&glibContext);
  DMD_updateDisplay();
}

/**
 * @brief Cập nhật màn hình LCD với dữ liệu mới.
 */
// Trong file lcd_display.c

void lcd_update(float temp, float hum, uint32_t sensor_period_ms)
{
  char temp_str[32];
  char hum_str[32];
  char period_str[32];

  // Tách số liệu để in (Tránh lỗi %f không hỗ trợ)
  int t_int = (int)temp;
  int t_dec = (int)((temp - t_int) * 10);
  if (t_dec < 0) t_dec = -t_dec;

  int h_int = (int)hum;
  int h_dec = (int)((hum - h_int) * 10);

  // Định dạng chuỗi bằng số nguyên
  sprintf(temp_str, "Nhiet do: %d.%d C", t_int, t_dec);
  sprintf(hum_str,  "Do am:    %d.%d %%", h_int, h_dec);
  sprintf(period_str, "Chu ky:  %lu ms", sensor_period_ms);

  // Vẽ lên bộ đệm GLIB
  GLIB_clear(&glibContext);
  GLIB_drawStringOnLine(&glibContext, temp_str, 1, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, hum_str, 3, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, period_str, 5, GLIB_ALIGN_LEFT, 5, 5, true);
  // 2. Hiển thị Tên & MSSV (Thêm vào Dòng 7 và 8)
    // Dùng font nhỏ nên tách dòng để không bị tràn màn hình
    GLIB_drawStringOnLine(&glibContext, "Luong Hai Long", 7, GLIB_ALIGN_LEFT, 5, 5, true);
    GLIB_drawStringOnLine(&glibContext, "MSSV: 22207056 F3", 8, GLIB_ALIGN_LEFT, 5, 5, true);

  // Đẩy bộ đệm ra màn hình
  DMD_updateDisplay();
}
