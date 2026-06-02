#include "lcd_display.h"
#include "glib.h"
#include "dmd.h"
#include <stdio.h>

// Global GLIB context.
static GLIB_Context_t glibContext;

/**
 * @brief Initialize the LCD module with DMD and GLIB.
 */
void lcd_init_and_clear(void)
{
  DMD_init(0);
  GLIB_contextInit(&glibContext);
  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;

  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8); // Use small font.
  GLIB_clear(&glibContext);
  DMD_updateDisplay();
}

/**
 * @brief Update the LCD screen with new sensor data.
 */
// LCD data update routine.

void lcd_update(float temp, float hum, uint32_t sensor_period_ms)
{
  char temp_str[32];
  char hum_str[32];
  char period_str[32];

  // Split numbers for printing because %f is not supported here.
  int t_int = (int)temp;
  int t_dec = (int)((temp - t_int) * 10);
  if (t_dec < 0) t_dec = -t_dec;

  int h_int = (int)hum;
  int h_dec = (int)((hum - h_int) * 10);

  // Format strings with integer values.
  sprintf(temp_str, "Temp:     %d.%d C", t_int, t_dec);
  sprintf(hum_str,  "Humidity: %d.%d %%", h_int, h_dec);
  sprintf(period_str, "Period:   %lu ms", sensor_period_ms);

  // Draw into the GLIB buffer.
  GLIB_clear(&glibContext);
  GLIB_drawStringOnLine(&glibContext, temp_str, 1, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, hum_str, 3, GLIB_ALIGN_LEFT, 5, 5, true);
  GLIB_drawStringOnLine(&glibContext, period_str, 5, GLIB_ALIGN_LEFT, 5, 5, true);
  // 2. Display name and student ID on lines 7 and 8.
    // Split the text into two lines so the small display does not overflow.
    GLIB_drawStringOnLine(&glibContext, "Luong Hai Long", 7, GLIB_ALIGN_LEFT, 5, 5, true);
    GLIB_drawStringOnLine(&glibContext, "ID: 22207056 F3", 8, GLIB_ALIGN_LEFT, 5, 5, true);

  // Push the buffer to the display.
  DMD_updateDisplay();
}
