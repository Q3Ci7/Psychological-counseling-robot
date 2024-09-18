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

#include "audio.h"
#include "ws2812b.h"
#include "uart.h"
#include "servo.h"
#include "HCSR.h"


// void cleanup_resources()
// {
//     // �ͷ� WS2812 ��Դ
//     ws2812_deinit(ws2812_handle);

//     // �ͷŶ���
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

//     // ��������
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

//     // �ͷ��ŷ������Դ
//     for (int i = 0; i < 3; ++i)  // ������3���ŷ�ͨ��
//     {
//         servo_deinit(i);
//     }

//     // �ͷų�������������Դ���������Ӧ���ͷź�����
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

#include "audio.h"
#include "ws2812b.h"
#include "uart.h"
#include "servo.h"
#include "HCSR.h"


// void cleanup_resources()
// {
//     // �ͷ� WS2812 ��Դ
//     ws2812_deinit(ws2812_handle);

//     // �ͷŶ���
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

//     // ��������
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

//     // �ͷ��ŷ������Դ
//     for (int i = 0; i < 3; ++i)  // ������3���ŷ�ͨ��
//     {
//         servo_deinit(i);
//     }

//     // �ͷų�������������Դ���������Ӧ���ͷź�����
//     // ultrasonic_deinit();
// }

>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
#endif // COMMON_H