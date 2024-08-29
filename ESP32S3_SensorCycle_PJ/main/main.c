/*
 ****************************************************
 *
 * @file:    main.c
 * @author:
 * @version:
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
 *          00001：
 *          00002：
 *          00003：
 *          00004：please look at the camera
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
#include "microphone.h"
#include "HCSR.h"

/************Audio Setting************/

const uint8_t volume = 10;
const uint8_t duration = 2;

/***********************************/

/************WS2812 Setting************/

ws2812_strip_handle_t ws2812_handle = NULL;
int idx = 0;         //
uint8_t lednum = 12; // Number of WS2812B_LED

uint8_t mod = 3;
/***********************************/

QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2; // Create queue handle

void sys_init()
{
    init_uart();                            // Serial port initialization
    ws2812_init(8, lednum, &ws2812_handle); // ws2812b initialization
    servo_init(0, 6);                       // servo initialization

    ultrasonic_init(); // ultrasonic initialization
    A_stop();
    ws2812_write(ws2812_handle, idx, 0, 0, 0); // Set initial RGB light color --  black(off)
}

void lightmode(uint8_t lednum)
{
    switch (mod)
    {
    // case 1: // 1 1 1 1 1 0 0 0 0
    //     for (uint8_t i = 0; i < lednum; i++)
    //     {
    //         ws2812_write(ws2812_handle, i, r, g, b);
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    //     for (uint8_t i = 0; i < lednum; i++)
    //     {
    //         ws2812_write(ws2812_handle, i, 0, 0, 0);
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    //     break;
    // case 2: // 0 0 0 0 0 1 0 0 0 0
    //     for (uint8_t i = 0; i < lednum; i++)
    //     {
    //         ws2812_write(ws2812_handle, i, r, g, b);
    //         vTaskDelay(pdMS_TO_TICKS(1000));
    //         ws2812_write(ws2812_handle, i, 0, 0, 0);
    //         vTaskDelay(pdMS_TO_TICKS(100));
    //     }
    //     break;
    case 3:
        for (uint8_t i = 1; i < lednum; i++)
        {
            switch (i)
            {
            case 1:
                ws2812_write(ws2812_handle, i, 0, 20, 0); // green
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 2:
                ws2812_write(ws2812_handle, i, 20, 20, 0); // yellow
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 3:
                ws2812_write(ws2812_handle, i, 25, 10, 0); // orange
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 4:
                ws2812_write(ws2812_handle, i, 25, 0, 0); // red
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 5:
                ws2812_write(ws2812_handle, i, 20, 0, 20); // purple
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 6:
                ws2812_write(ws2812_handle, i, 0, 0, 20); // deepblue
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            case 7:
                ws2812_write(ws2812_handle, i, 3, 14, 25); // lightblue
                vTaskDelay(pdMS_TO_TICKS(200));
                break;
            }
        }
        break;
    default:
        break;
    }
}

void shake()
{
    float distance;
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度

    for (uint8_t i = 0; i <= 120; i++)
    {
        set_servo_angle(0, i); // 假设 `set_servo_angle` 函数控制舵机角度

        // 等待舵机移动到目标角度
        vTaskDelay(200 / portTICK_PERIOD_MS); // 500 毫秒等待时间（根据实际情况调整）
        if (i != 0)
        {
            distance = ultrasonic_get_distance(); // 获取当前角度的距离
            printf("Angle: %d°, Distance: %.2f cm\n", i, distance);
            // 记录最小距离及其对应的角度
            if (distance >= 0 && distance < best_distance)
            {
                best_distance = distance;
                best_angle = i;
            }
        }
    }

    // 将舵机转到最佳角度
    set_servo_angle(0, best_angle);
    printf("Best Angle: %d°, Best Distance: %.2f cm\n", best_angle, best_distance);
}

void app_main()
{

    sys_init(); // System initialization

    /**************CREATE QUEUE**************/
    // data_queue1 = xQueueCreate(10, sizeof(bpm_spo2_t));
    // if (data_queue1 == NULL)
    // {
    //     ESP_LOGE("Queue", "Queue1 creation failed!");
    // }
    // data_queue2 = xQueueCreate(10, sizeof(Message));
    // if (data_queue2 == NULL)
    // {
    //     ESP_LOGE("Queue", "Queue2 creation failed!");
    // }
    /***********************************/

    /************FOR TSET**********/
    // ws2812_write(ws2812_handle, idx, 80, 80, 0);
    // lightmode(0, 0, 20);
    // Audio_init(1, 10);
    // A_choose(2);
    // shake();
    // warm_begin();

    /***********************************/

    /**************CREATE TASK**************/
    // xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);         // Serial port receives data
    /***********************************/

    while (1)
    {
        lightmode(4);
        vTaskDelay(pdMS_TO_TICKS(500));
    }

    // ws2812_deinit(ws2812_handle);
}
