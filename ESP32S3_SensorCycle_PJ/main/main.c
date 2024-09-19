/*
 *
 * @file:
 *
 * @author: Ricky
 *
 * @date:
 *
 * @version:
 *
 * @description:
 *
 *
 */

#include "common.h"

/************Audio Setting**********/
const uint8_t music_volume = 20;
const uint8_t voice_volume = 25;
const uint8_t duration = 2;

/************WS2812 Setting************/
uint8_t num = 0; // light mode choose
ws2812_strip_handle_t ws2812_handle = NULL;
int idx = 0;         //
uint8_t lednum = 12; // Number of WS2812B_LED

// QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2;   // Create queue handle
QueueHandle_t data_queue3;   // Create queue handle
QueueHandle_t distanceQueue; // Create distance queue handle

static TaskHandle_t check_task_handle = NULL; // check play station task handle
volatile bool task_control_flag = false;      // check play station task contol flag
volatile uint8_t end_flag = 0;
volatile bool shake_flag = false; // 用于控制

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
            ws2812_write(ws2812_handle, 0, 24, 16, 0); // warm
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 24, 15, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 24, 14, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 24, 13, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 24, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 24, 11, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 24, 10, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 24, 9, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 24, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 24, 7, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 6, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "1");
            break;
        case 2:
            lightreset();
            ws2812_write(ws2812_handle, 0, 24, 24, 0); // happy
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 24, 22, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 24, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 24, 18, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 24, 16, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 24, 14, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 24, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 24, 10, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 24, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 24, 7, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 6, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "2");
            break;
        case 3:
            lightreset();
            ws2812_write(ws2812_handle, 0, 0, 24, 0); // calm
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 4, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 8, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 12, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 16, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 24, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 24, 20, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 24, 16, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 24, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 24, 8, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 6, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "3");
            break;
        case 4:
            lightreset();
            ws2812_write(ws2812_handle, 0, 0, 24, 24); // sad
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 0, 24, 18);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 0, 24, 12);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 0, 24, 6);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 6, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 12, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 18, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 24, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 24, 18, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "4");
            break;
        case 5:
            lightreset();
            ws2812_write(ws2812_handle, 0, 0, 0, 24); // afraid
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 0, 8, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 0, 16, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 0, 24, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 24, 16);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 0, 24, 8);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 0, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 8, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 16, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 24, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 18, 2);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "5");
            break;
        case 6:
            lightreset();
            ws2812_write(ws2812_handle, 0, 12, 0, 24); // scared
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 6, 0, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 0, 0, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 0, 12, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 24, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 8, 24, 16);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 16, 24, 8);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 24, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 12, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 0, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 12, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "6");
            break;
        case 7:
            lightreset();
            ws2812_write(ws2812_handle, 0, 24, 0, 0); // anger
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 1, 18, 0, 12);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 2, 12, 0, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 3, 6, 0, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 4, 0, 0, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 5, 0, 12, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 6, 0, 24, 24);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 7, 0, 24, 12);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 8, 0, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 9, 12, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 10, 24, 24, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ws2812_write(ws2812_handle, 11, 24, 5, 0);
            vTaskDelay(pdMS_TO_TICKS(delaytime));
            ESP_LOGI("lgihttyepe", "7");
            break;
        case 8:
            lightreset();
            break;
        case 9:
            lighton();
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void shake()
{
    uint8_t count1 = 0;
    bool flip_flag = false;
    float distance;
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度
    double step = 0.05;            // 每个插值点的时间间隔
    double current_time = 0;
    double target_angle = 100;
    // servo_smooth_move(0, 52, 0, 2);
    set_servo_angle(0, 0);
    while (current_time <= 4.5 && shake_flag)
    {
        ESP_LOGI("current_time", "%.2f", current_time); // 日志打印shake函数运行状态
        double angle = 0 + ((target_angle - 0) * (current_time / 4.5));
        set_servo_angle(0, angle);
        count1++;

        distance = ultrasonic_get_distance(); // 获取当前角度的距离
        // printf("distance:%.2fcm\n", distance);
        if (distance >= 0 && distance < best_distance) // 记录最小距离及其对应的角度
        {
            best_distance = distance;
            best_angle = angle;
        }

        if (count1 <= 30)
        {
            uint8_t arm_angle = flip_flag ? count1 : (30 - count1);
            set_servo_angle(2, arm_angle);
            set_servo_angle(1, 30 - arm_angle);
        }
        else
        {
            count1 = 0;
            flip_flag = !flip_flag; // 翻转标志位
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        current_time += step;
    }

    // 将头部舵机转到最佳角度
    // servo_smooth_move(0, 100, best_angle, 2);
    set_servo_angle(0, best_angle);
    shake_flag = false;
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
                shake_flag = true; // 使能shake
                Audio_init(2, voice_volume);
                A_choose(1);
                shake();
                end_flag = 0;
                task_control_flag = true;
                ESP_LOGI("msgtype", "1");
                break;
            case 2:
                Audio_init(2, voice_volume);
                A_choose(2);
                end_flag = 1;
                task_control_flag = true;
                ESP_LOGI("msgtype", "2");
                break;
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            case 13:
            case 14:
                num = 8; // 灯光全灭状态
                Audio_init(2, music_volume);
                A_choose(msg.msgtype);
                end_flag = 3;
                task_control_flag = true;
                ESP_LOGI("msgtype", "audiotype");
                break;
            case 15:
            case 16:
                num = 9; // 灯光全亮状态
                A_stop();
                task_control_flag = false;
                ESP_LOGI("msgtype", "16");
                break;
            default:
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
    servo_init(0, 6);                       // servo 0 initialization
    servo_init(1, 41);                      // servo 1 initialization
    servo_init(2, 42);                      // servo 2 initialization
    ultrasonic_init();                      // ultrasonic initialization
    // A_stop();
    set_servo_angle(0, 52); // Initialize the header Angle
    lightbegin(2);
}

void app_main()
{
    sys_init(); // System initialization
    /**************CREATE QUEUE**************/
    data_queue2 = xQueueCreate(10, sizeof(Message));
    data_queue3 = xQueueCreate(10, sizeof(EMO));
    distanceQueue = xQueueCreate(10, sizeof(float));
    if (data_queue2 == NULL || data_queue3 == NULL || distanceQueue == NULL)
    {
        ESP_LOGE("Queue", "Queue creation failed!");
        return;
    }

    /**************CREATE TASK**************/
    xTaskCreate(lightmode_task, "lightmode_task", 4096, NULL, 5, NULL);
    // xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);
    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 10, NULL);
    xTaskCreate(psychic_run_task, "psychic_run_task", 4096, NULL, 5, NULL);
    xTaskCreate(checkplystation, "checkplystation", 4096, NULL, 5, &check_task_handle);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ws2812_deinit(ws2812_handle);
}
