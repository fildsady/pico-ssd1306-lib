#ifndef U8G2_PICO_HAL_H
#define U8G2_PICO_HAL_H
#include "u8g2.h"

/* Pico SDK + FreeRTOS HAL for u8g2 — non-blocking DMA-to-I2C. Does NOT own
 * any hardware itself: every transfer is funneled through
 * ssd1306_i2c_send_dma() (ssd1306.c/.h), the single shared DMA
 * channel/semaphore for OLED_I2C_PORT. u8g2 has its own SSD1306 driver +
 * framebuffer (doesn't share rendering code with ssd1306.c/font.h), but both
 * paths now go through the same low-level sender, so they're safe to call
 * from the same task on the same physical display — see ssd1306.h for why
 * that matters (two independent DMA/semaphore owners on one I2C peripheral
 * caused a real hang).
 *
 * REQUIRED: call ssd1306_init() once before using this HAL — it owns
 * I2C/DMA setup. This HAL's U8X8_MSG_BYTE_INIT does not init I2C itself.
 *
 * Requires FreeRTOS (FreeRTOS.h/task.h) — same dependency as ssd1306.c, not
 * intended for bare-metal use.
 *
 * Usage (from within a FreeRTOS task, after ssd1306_init()):
 *   u8g2_t u8g2;
 *   u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0,
 *       u8g2_pico_hal_byte_cb, u8g2_pico_hal_gpio_and_delay_cb);
 *   u8g2_InitDisplay(&u8g2);
 *   u8g2_SetPowerSave(&u8g2, 0);
 *   u8g2_ClearBuffer(&u8g2);
 *   u8g2_DrawStr(&u8g2, 0, 20, "Hello");
 *   u8g2_SendBuffer(&u8g2);
 */
uint8_t u8g2_pico_hal_byte_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8g2_pico_hal_gpio_and_delay_cb(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);

#endif
