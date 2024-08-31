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
// #include "gpio_ctrl.h"
// #include "bpm.h"
#include "uart.h"
#include "servo.h"
// #include "emotion.h"
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

/***********************************/

QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2; // Create queue handle

void lightbegin(uint8_t mode)
{
    uint8_t i;
    switch (mode)
    {
    case 1: // 单色 渐变 点亮熄灭3次
        for (i = 1; i < 4; i++)
        {
            for (i = 1; i < 4; i++)
            {
                ws2812_fade(ws2812_handle, 0, 11, 0, 0, 0, 50, 0, 0, 200, 50); // 使用200步
                vTaskDelay(pdMS_TO_TICKS(500));
                ws2812_fade(ws2812_handle, 0, 11, 50, 0, 0, 0, 0, 0, 200, 50); // 使用200步
                vTaskDelay(pdMS_TO_TICKS(500));
            }
        }
        break;
    case 2: // 单色 循环依次点亮 再全部灭亮3次
        for (i = 0; i < 12; i++)
        {
            ws2812_write(ws2812_handle, i, 10, 10, 10);
            vTaskDelay(pdMS_TO_TICKS(50));
        }
        for (uint8_t j = 0; j < 3; j++)
        {
            for (i = 0; i < 12; i++)
            {
                ws2812_write(ws2812_handle, i, 0, 0, 0);
            }
            vTaskDelay(pdMS_TO_TICKS(500));
            for (i = 0; i < 12; i++)
            {
                ws2812_write(ws2812_handle, i, 10, 10, 10);
            }
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        break;
    default: // 全部熄灭
        for (i = 1; i < 12; i++)
        {
            ws2812_write(ws2812_handle, i, 0, 0, 0);
        }
        break;
    }
}

void lightreset()
{
    for (uint8_t i = 0; i < 12; i++)
    {
        ws2812_write(ws2812_handle, i, 0, 0, 0);
    }
}

void lightadd(uint8_t num, uint8_t r, uint8_t g, uint8_t b)
{
    for (uint8_t i = 0; i < num; i++)
    {
        ws2812_write(ws2812_handle, i, r, g, b);
    }
}

void lightadd2(uint8_t num)
{
    if (num >= 0)
    {
        ws2812_write(ws2812_handle, 0, 0, 20, 0);
    }
    if (num >= 1)
    {
        ws2812_write(ws2812_handle, 1, 20, 20, 0);
    }
    if (num >= 2)
    {
        ws2812_write(ws2812_handle, 2, 25, 10, 0);
    }
    if (num >= 3)
    {
        ws2812_write(ws2812_handle, 3, 25, 0, 0);
    }
    if (num >= 4)
    {
        ws2812_write(ws2812_handle, 4, 20, 0, 20);
    }
    if (num >= 5)
    {
        ws2812_write(ws2812_handle, 5, 0, 0, 20);
    }
    if (num >= 6)
    {
        ws2812_write(ws2812_handle, 6, 3, 14, 25);
    }
}

void lightmode_task(void *pvParameters)
{
    uint8_t i;
    Message msg;
    while (1)
    {
        if (xQueueReceive(data_queue2, &msg, 0) == pdPASS)
        {
            for (i = 1; i < msg.msgtype; i++)
            {
                switch (i)
                {
                case 1:

                    lightadd(i, 0, 20, 0); // green
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 2:

                    lightadd(i, 20, 20, 0); // yellow
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 3:

                    lightadd(i, 25, 10, 0); // orange
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 4:

                    lightadd(i, 25, 0, 0); // red
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 5:

                    lightadd(i, 20, 0, 20); // purple
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 6:

                    lightadd(i, 0, 0, 20); // deepblue
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                case 7:

                    lightadd(i, 3, 14, 25); // lightblue
                    vTaskDelay(pdMS_TO_TICKS(200));
                    break;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void shake()
{
    float distance;
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度
    for (uint8_t i = 0; i <= 120; i += 5)
    {
        set_servo_angle(0, i); // 假设 `set_servo_angle` 函数控制舵机角度

        // 等待舵机移动到目标角度
        vTaskDelay(300 / portTICK_PERIOD_MS); // 500 毫秒等待时间（根据实际情况调整）
        if (i != 0)
        {
            distance = ultrasonic_get_distance(); // 获取当前角度的距离
            // printf("Angle: %d°, Distance: %.2f cm\n", i, distance);
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
    // printf("Best Angle: %d°, Best Distance: %.2f cm\n", best_angle, best_distance);
}

void psychic_run_task(void *pvParameters)
{
    Message msg;
    while (1)
    {
        if (xQueueReceive(data_queue2, &msg, 0) == pdPASS)
        {
            switch (msg.msgtype)
            {
            case 1:
                Audio_init(2, 30);
                A_choose(1);
                shake();
                uart_write_bytes(UART_NUM_2, "selfend", strlen("selfend"));
                break;
            case 2:
                Audio_init(2, 30);
                A_choose(2);
                break;
            case 3:
                lightreset();
                lightadd2(msg.msgtype - 3); // green
                break;
            case 4:
                lightreset();
                lightadd2(msg.msgtype - 3); // green // yellow
                break;
            case 5:
                lightreset();
                lightadd2(msg.msgtype - 3); // green// orange
                break;
            case 6:
                lightreset();
                lightadd2(msg.msgtype - 3); // green // red
                break;
            case 7:
                lightreset();
                lightadd2(msg.msgtype - 3); // green // purple
                break;
            case 8:
                lightreset();
                lightadd2(msg.msgtype - 3); // green // deepblue
                break;
            case 9:
                lightreset();
                lightadd2(msg.msgtype - 3); // green // lightblue
                break;
            case 10:
                Audio_init(2, 10);
                A_choose(3);
                checkplystation();
                break;
            case 11:
                Audio_init(2, 10);
                A_choose(4);
                checkplystation();
                break;
            case 12:
                Audio_init(2, 10);
                A_choose(5);
                checkplystation();
                break;
            case 13:
                Audio_init(2, 10);
                A_choose(6);
                checkplystation();
                break;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void sys_init()
{
    init_uart();                            // Serial port initialization
    ws2812_init(8, lednum, &ws2812_handle); // ws2812b initialization
    servo_init(0, 6);                       // servo initialization

    ultrasonic_init(); // ultrasonic initialization
    A_stop();
    lightbegin(2);
}





void app_main()
{

    sys_init(); // System initialization

    /**************CREATE QUEUE**************/
    data_queue2 = xQueueCreate(10, sizeof(Message));
    if (data_queue2 == NULL)
    {
        ESP_LOGE("Queue", "Queue2 creation failed!");
    }
    /***********************************/

    /************FOR TSET**********/
    // ws2812_write(ws2812_handle, idx, 80, 80, 0);
    // lightmode(0, 0, 20);
    Audio_init(2, 10);
    A_choose(3);
    checkplystation();

    // shake();
    // warm_begin();

    /***********************************/

    /**************CREATE TASK**************/
    // xTaskCreate(lightmode_task, "lightmode_task", 4096, NULL, 5, NULL);     // Serial port receives data

    // xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);         // Serial port receives data
    // xTaskCreate(psychic_run_task, "psychic_run_task", 4096, NULL, 5, NULL); // Serial port receives data
    /***********************************/

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // ws2812_deinit(ws2812_handle);
}
