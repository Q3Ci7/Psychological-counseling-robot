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
// #include "microphone.h"
#include "HCSR.h"

/************Audio Setting************/

const uint8_t music_volume = 5;
const uint8_t voice_volume = 5;
const uint8_t duration = 2;

/***********************************/

/************WS2812 Setting************/
uint8_t num = 0; // light mode choose
ws2812_strip_handle_t ws2812_handle = NULL;
int idx = 0;         //
uint8_t lednum = 12; // Number of WS2812B_LED

/***********************************/

// QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2; // Create queue handle
QueueHandle_t data_queue3; // Create queue handle
static TaskHandle_t check_task_handle = NULL;
volatile bool task_control_flag = false;

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
    const uint8_t delaytime = 100;
    EMO emo;
    while (1)
    {
        if (xQueueReceive(data_queue3, &emo, 0) == pdPASS)
        {
            num = emo.emotype;
        }
        switch (num)
        {
        case 1:
            lightreset();
            //                             r  g  b
            ws2812_write(ws2812_handle, 0, 0, 2, 0); // g
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 0, 4, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 0, 6, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 0, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 10, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 0, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 0, 14, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 0, 16, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 0, 18, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 0, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 0, 22, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 24, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 2:
            lightreset();
            ws2812_write(ws2812_handle, 0, 20, 20, 0); // y
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 19, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 18, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 16, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 14, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 12, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 10, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 8, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 6, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 4, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 2, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 20, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 3:
            lightreset();
            ws2812_write(ws2812_handle, 0, 25, 2, 0); // o
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 22, 4, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 20, 6, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 18, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 16, 10, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 14, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 12, 14, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 10, 16, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 8, 18, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 6, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 4, 22, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 24, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 4:
            lightreset();
            ws2812_write(ws2812_handle, 0, 25, 0, 0); // r
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 22, 4, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 20, 6, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 18, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 16, 10, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 14, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 12, 14, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 10, 16, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 8, 18, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 6, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 4, 22, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 24, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 5:
            lightreset();
            ws2812_write(ws2812_handle, 0, 24, 0, 24); // p
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 22, 2, 22);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 20, 4, 20); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 18, 6, 18);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 16, 8, 16); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 14, 10, 15);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 12, 12, 14); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 10, 14, 8);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 8, 16, 6); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 6, 18, 4);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 4, 20, 2); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 22, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 6:
            lightreset();
            ws2812_write(ws2812_handle, 0, 0, 0, 24); // db
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 0, 2, 22);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 0, 4, 20); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 0, 6, 18);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 8, 16); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 0, 10, 14);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 0, 12, 12); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 0, 14, 10);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 0, 16, 8); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 0, 18, 6);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 0, 20, 4); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 22, 0); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 7:
            lightreset();
            ws2812_write(ws2812_handle, 0, 10, 10, 20); //
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 0, 0, 20); // db
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 10, 0, 20);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 20, 0, 20); // p
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 20, 0, 10);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 20, 0, 0); // r
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 20, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 20, 10, 0); // o
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 20, 15, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 20, 20, 0); // y
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 10, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 0, 20, 0); // g
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            break;
        case 8:
            lightreset();
            break;
        default:

            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#if 0
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

#elif 0
void shake()
{
    uint8_t count1 = 0;
    uint8_t count2 = 0;
    bool flip_flag = 0;
    float distance;
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度
    for (uint8_t i = 0; i <= 120; i++)
    {
        count1++;
        count2 += 5;
        if (count1 <= 30)
        {
            if (flip_flag)
            {
                set_servo_angle(2, count1);
                set_servo_angle(1, count1);
                vTaskDelay(pdMS_TO_TICKS(10)); // 控制速度，10ms延时
            }
            else
            {
                set_servo_angle(2, 30 - count1);
                set_servo_angle(1, 30 - count1);
                vTaskDelay(pdMS_TO_TICKS(10)); // 控制速度，10ms延时
            }
        }
        else
        {
            count1 = 0;             // count1置0
            flip_flag = !flip_flag; // 翻转标志位--表示反向
        }
        //***********************************************************************/
        if (i % 5 == 0)
        {
            set_servo_angle(0, i); // 假设 `set_servo_angle` 函数控制舵机角度

            // 等待舵机移动到目标角度
            vTaskDelay(200 / portTICK_PERIOD_MS); // 500 毫秒等待时间（根据实际情况调整）
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
        //***********************************************************************/
    }

    set_servo_angle(0, best_angle); // 将舵机转到最佳角度
    // printf("Best Angle: %d°, Best Distance: %.2f cm\n", best_angle, best_distance);
}

#else
void shake()
{
    uint8_t count1 = 0;
    uint8_t count2 = 0;
    bool flip_flag = false;
    float distance;
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度

    for (uint8_t i = 0; i <= 120; i++)
    {
        count1++;
        count2 += 5;

        if (count1 <= 30)
        {
            uint8_t angle = flip_flag ? count1 : (30 - count1);

            set_servo_angle(2, angle);      // 手部舵机
            set_servo_angle(1, 30 - angle); // 头部舵机
            vTaskDelay(pdMS_TO_TICKS(50));   // 5ms延时
        }
        else
        {
            count1 = 0;
            flip_flag = !flip_flag; // 翻转标志位
        }

        // 每5步调整一次主舵机的位置
        if (i % 5 == 0)
        {
            set_servo_angle(0, i);          // 调整头部舵机角度
            vTaskDelay(pdMS_TO_TICKS(100)); // 减少延迟，加快速度

            if (i != 0)
            {
                distance = ultrasonic_get_distance(); // 获取当前角度的距离
                // printf("distance:%.2fcm\n", distance);
                // 记录最小距离及其对应的角度
                if (distance >= 0 && distance < best_distance)
                {
                    best_distance = distance;
                    best_angle = i;
                }
            }
        }
    }

    // 将头部舵机转到最佳角度
    set_servo_angle(0, best_angle);
}
#endif

//***************************************************************** */
void resume_check_task()
{
    if (check_task_handle != NULL)
    {
        // Check if the task is suspended before resuming
        if (eTaskGetState(check_task_handle) == eSuspended)
        {
            vTaskResume(check_task_handle);
            ESP_LOGI("task_control", "Task resumed");
        }
        else
        {
            ESP_LOGI("task_control", "Task is already running or not suspended");
        }
    }
}

void suspend_check_task()
{
    if (check_task_handle != NULL)
    {
        // Check if the task is running before suspending
        if (eTaskGetState(check_task_handle) == eRunning)
        {
            vTaskSuspend(check_task_handle);
            ESP_LOGI("task_control", "Task suspended");
        }
        else
        {
            ESP_LOGI("task_control", "Task is already suspended or not running");
        }
    }
}
//***************************************************************** */

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
                Audio_init(2, voice_volume);
                A_choose(1);
                shake();
                uart_write_bytes(UART_NUM_2, "selfend", strlen("selfend"));
                ESP_LOGI("msgtype", "1");
                break;
            case 2:
                Audio_init(2, voice_volume);
                A_choose(2);
                ESP_LOGI("msgtype", "2");
                break;
            case 10:
            case 11:
            case 12:
            case 13:
                num = 8;
                Audio_init(2, music_volume);
                A_choose(msg.msgtype - 7);

                vTaskDelay(pdMS_TO_TICKS(1000));
                task_control_flag = true;
                // resume_check_task(); // 恢复
                ESP_LOGI("msgtype", "%d", msg.msgtype);
                break;
            case 14:
                num = 8;
                // suspend_check_task(); // 挂起
                task_control_flag = false;
                vTaskDelay(pdMS_TO_TICKS(1000));
                A_stop();
                ESP_LOGI("msgtype", "14");
                break;
            default:
                // num = 0;
                // A_stop();
                // suspend_check_task(); // 挂起
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
    servo_init(1, 41);
    servo_init(2, 42);
    ultrasonic_init(); // ultrasonic initialization
    // A_stop();
    lightbegin(2);
}

void app_main()
{
    sys_init(); // System initialization

    /**************CREATE QUEUE**************/
    data_queue2 = xQueueCreate(10, sizeof(Message));
    data_queue3 = xQueueCreate(10, sizeof(EMO));
    if (data_queue2 == NULL || data_queue3 == NULL)
    {
        ESP_LOGE("Queue", "Queue creation failed!");
        return;
    }
    /***********************************/

    /**************Create a control semaphore**************/
    // control_semaphore = xSemaphoreCreateBinary();
    // if (control_semaphore == NULL)
    // {
    //     ESP_LOGE("app_main", "Failed to create semaphore");
    //     return;
    // }
    /***********************************/

    /************FOR TSET**********/
    // ws2812_write(ws2812_handle, idx, 80, 80, 0);
    // lightmode(0, 0, 20);
    // Audio_init(2, 5);
    // A_choose(3);
    // checkplystation();

    // shake();
    // warm_begin();

    /***********************************/

    /**************CREATE TASK**************/
    xTaskCreate(lightmode_task, "lightmode_task", 4096, NULL, 5, NULL);     // Serial port receives data
    xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);         // Serial port receives data
    xTaskCreate(psychic_run_task, "psychic_run_task", 4096, NULL, 5, NULL); // Serial port receives data
    xTaskCreate(checkplystation, "checkplystation", 4096, NULL, 5, &check_task_handle);
    /***********************************/

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ws2812_deinit(ws2812_handle);
}
