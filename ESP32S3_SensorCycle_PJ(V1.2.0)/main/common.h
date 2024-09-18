#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/adc.h"
// #include "driver/rmt.h"    //ÒÑ±»ÆúÓÃ
#include "driver/rmt_tx.h"
#include "driver/ledc.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_err.h"


#include "../include/uart.h"
#include "../max30102.h"



#endif // COMMON_H