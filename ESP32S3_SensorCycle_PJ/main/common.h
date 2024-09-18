<<<<<<< HEAD
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"

// #include "driver/adc.h"
#include "esp_adc/adc_continuous.h" // 头文件引用路径由 driver/adc.h 更新为 esp_adc/adc_continuous.h
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
#include <float.h> // 包含 FLT_MAX

#include "audio.h"
#include "ws2812b.h"
#include "uart.h"
#include "servo.h"
#include "HCSR.h"


// void cleanup_resources()
// {
//     // 释放 WS2812 资源
//     ws2812_deinit(ws2812_handle);

//     // 释放队列
//     if (data_queue2 != NULL)
//     {
//         vQueueDelete(data_queue2);
//         data_queue2 = NULL;
//     }
//     if (data_queue3 != NULL)
//     {
//         vQueueDelete(data_queue3);
//         data_queue3 = NULL;
//     }
//     if (distanceQueue != NULL)
//     {
//         vQueueDelete(distanceQueue);
//         distanceQueue = NULL;
//     }

//     // 结束任务
//     if (lightmode_task_handle != NULL)
//     {
//         vTaskDelete(lightmode_task_handle);
//         lightmode_task_handle = NULL;
//     }
//     if (rx_uart_task_handle != NULL)
//     {
//         vTaskDelete(rx_uart_task_handle);
//         rx_uart_task_handle = NULL;
//     }
//     if (psychic_run_task_handle != NULL)
//     {
//         vTaskDelete(psychic_run_task_handle);
//         psychic_run_task_handle = NULL;
//     }
//     if (check_task_handle != NULL)
//     {
//         vTaskDelete(check_task_handle);
//         check_task_handle = NULL;
//     }

//     // 释放伺服电机资源
//     for (int i = 0; i < 3; ++i)  // 假设有3个伺服通道
//     {
//         servo_deinit(i);
//     }

//     // 释放超声波传感器资源（如果有相应的释放函数）
//     // ultrasonic_deinit();
// }

=======
#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"
#include "driver/gpio.h"

// #include "driver/adc.h"
#include "esp_adc/adc_continuous.h" // 头文件引用路径由 driver/adc.h 更新为 esp_adc/adc_continuous.h
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
#include <float.h> // 包含 FLT_MAX

#include "audio.h"
#include "ws2812b.h"
#include "uart.h"
#include "servo.h"
#include "HCSR.h"


// void cleanup_resources()
// {
//     // 释放 WS2812 资源
//     ws2812_deinit(ws2812_handle);

//     // 释放队列
//     if (data_queue2 != NULL)
//     {
//         vQueueDelete(data_queue2);
//         data_queue2 = NULL;
//     }
//     if (data_queue3 != NULL)
//     {
//         vQueueDelete(data_queue3);
//         data_queue3 = NULL;
//     }
//     if (distanceQueue != NULL)
//     {
//         vQueueDelete(distanceQueue);
//         distanceQueue = NULL;
//     }

//     // 结束任务
//     if (lightmode_task_handle != NULL)
//     {
//         vTaskDelete(lightmode_task_handle);
//         lightmode_task_handle = NULL;
//     }
//     if (rx_uart_task_handle != NULL)
//     {
//         vTaskDelete(rx_uart_task_handle);
//         rx_uart_task_handle = NULL;
//     }
//     if (psychic_run_task_handle != NULL)
//     {
//         vTaskDelete(psychic_run_task_handle);
//         psychic_run_task_handle = NULL;
//     }
//     if (check_task_handle != NULL)
//     {
//         vTaskDelete(check_task_handle);
//         check_task_handle = NULL;
//     }

//     // 释放伺服电机资源
//     for (int i = 0; i < 3; ++i)  // 假设有3个伺服通道
//     {
//         servo_deinit(i);
//     }

//     // 释放超声波传感器资源（如果有相应的释放函数）
//     // ultrasonic_deinit();
// }

>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
#endif // COMMON_H