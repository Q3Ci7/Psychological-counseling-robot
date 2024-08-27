/*
 ****************************************************
 *
 * @file:    main.c
 * @author:  JAYWAlKER
 * @version: V1.0.1
 * @date:    2024/7/24
 *
 ****************************************************
 * MAX30102:
 *          SCL === 10
 *          SDA === 9
 *
 * Relay:   (is used to control heating switchs)
 *          pin === 2
 *
 * UART_2:  (is used to receive sentiment strings)
 *          TX === 11  ===> RX
 *          RX === 12  ===> TX
 *
 * JQ8900: ( - Voice broadcast module;
 *           - Serial port control;  )
 *          RX ===> TX
 *
 *          00001£º
 *          00002£º
 *          00003£º
 *          00004£ºplease look at the camera
 *
 * WS2812:
 *          S === 8
 *
 *
 *
 *
 *
 */

#include "common.h"
#include "audio.h"
#include "ws2812b.h"
// #include "gsr.h"
#include "gpio_ctrl.h"
#include "bpm.h"
#include "uart.h"
#include "servo.h"
#include "emotion.h"

/************Audio Setting************/

const uint8_t volume = 25;
const uint8_t duration = 2;

/***********************************/

/************WS2812 Setting************/

ws2812_strip_handle_t ws2812_handle = NULL;
int idx = 0;
uint8_t lednum = 1; // Number of WS2812B_LED

/***********************************/


QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2; // Create queue handle

void sys_init()
{
    init_uart();                            // Serial port initialization
    ws2812_init(8, lednum, &ws2812_handle); // ws2812b initialization
    mygpio_init();                          // GPIO initialization
    // i2c_init();                                  // I2C  initialization (If not, please comment)  
    servo_init(0, 5);

    A_stop();
    warm_stop();                               // Set initial heating function off
    ws2812_write(ws2812_handle, idx, 0, 0, 0); // Set initial RGB light color --  black(off)
}




void app_main()
{

    sys_init(); // System initialization

    /**************CREATE QUEUE**************/
    data_queue1 = xQueueCreate(10, sizeof(bpm_spo2_t));
    if (data_queue1 == NULL)
    {
        ESP_LOGE("Queue", "Queue1 creation failed!");
    }
    data_queue2 = xQueueCreate(10, sizeof(Message));
    if (data_queue2 == NULL)
    {
        ESP_LOGE("Queue", "Queue2 creation failed!");
    }
    /***********************************/

    /************FOR TSET**********/
    // ws2812_write(ws2812_handle, idx, 250, 50, 90);
    // A_choose(3);

    set_servo_angle(0,0);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(0,45);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(0,90);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(0,135);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(0,180);
    vTaskDelay(pdMS_TO_TICKS(1000));
    set_servo_angle(0,90);
    vTaskDelay(pdMS_TO_TICKS(1000));

    // warm_begin();

    /***********************************/

    /**************CREATE TASK**************/
    // xTaskCreate(get_bpm_task, "get_bpm_task", 4096, NULL, 5, NULL);         // Get heart rate data
    // xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);         // Serial port receives data
    // xTaskCreate(sensorCycle_task, "sensorCycle_task", 8192, NULL, 5, NULL); // Control execution
    /***********************************/

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
