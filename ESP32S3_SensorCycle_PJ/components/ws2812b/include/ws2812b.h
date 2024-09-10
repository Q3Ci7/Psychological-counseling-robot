/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"

#define INPUT_FLAG 1 //输入方式控制 rgb形式  / 16进制形式


#ifdef __cplusplus
extern "C" {
#endif


typedef struct ws2812_strip_t *ws2812_strip_handle_t;
extern ws2812_strip_handle_t ws2812_handle;


esp_err_t ws2812_init(gpio_num_t gpio,int maxled,ws2812_strip_handle_t* led_handle);
esp_err_t ws2812_deinit(ws2812_strip_handle_t handle);

#if INPUT_FLAG
esp_err_t ws2812_write(ws2812_strip_handle_t handle,uint32_t index,uint32_t r,uint32_t g,uint32_t b);
#else 
esp_err_t ws2812_write(ws2812_strip_handle_t handle, uint32_t index, uint32_t hex_color)
#endif


esp_err_t ws2812_fade(ws2812_strip_handle_t handle, uint32_t start_index, uint32_t end_index, uint32_t start_r, uint32_t start_g, uint32_t start_b, uint32_t end_r, uint32_t end_g, uint32_t end_b, uint32_t steps, uint32_t delay_ms);
void lightbegin(uint8_t mode);
void lightreset();
void lightadd(uint8_t num, uint8_t r, uint8_t g, uint8_t b);



#ifdef __cplusplus
}
#endif
