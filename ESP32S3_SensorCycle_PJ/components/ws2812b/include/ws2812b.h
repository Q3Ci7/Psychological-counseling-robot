/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#include <stdint.h>
#include "driver/rmt_encoder.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct ws2812_strip_t *ws2812_strip_handle_t;

/** ��ʼ��WS2812����
 * @param gpio ����WS2812�Ĺܽ�
 * @param maxled ����WS2812�ĸ���
 * @param led_handle ���صĿ��ƾ��
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_init(gpio_num_t gpio,int maxled,ws2812_strip_handle_t* led_handle);

/** ����ʼ��WS2812����
 * @param handle ��ʼ���ľ��
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_deinit(ws2812_strip_handle_t handle);

/** ��ĳ��WS2812д��RGB����
 * @param handle ���
 * @param index �ڼ���WS2812��0��ʼ��
 * @param r,g,b RGB����
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_write(ws2812_strip_handle_t handle,uint32_t index,uint32_t r,uint32_t g,uint32_t b);


#ifdef __cplusplus
}
#endif
