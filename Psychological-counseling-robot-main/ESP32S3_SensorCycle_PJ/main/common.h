#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"

// #include "driver/adc.h"
#include "esp_adc/adc_continuous.h" // ͷ�ļ�����·���� driver/adc.h ����Ϊ esp_adc/adc_continuous.h
// #include "driver/rmt.h"   
#include "driver/rmt_tx.h"
#include "driver/ledc.h"
#include "driver/i2c.h"
#include "driver/i2s.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"


#include "../include/uart.h"
#include "../max30102.h"

#include <float.h> // ���� FLT_MAX

#endif // COMMON_H