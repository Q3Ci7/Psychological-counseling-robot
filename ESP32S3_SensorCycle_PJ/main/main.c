/*
 *
 * @file:
 *
 * @author: Ricky Yi
 *
 * @version:
 *
 * @date:
 *
 * @description:
 *
 *
 */

#include "common.h"
#include "audio.h"
#include "ws2812b.h"
#include "uart.h"
#include "servo.h"
#include "HCSR.h"

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
volatile bool end_flag = true;                // check play station task contol flag

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
            break;
        case 8:
            lightreset();
            break;
        case 9:
            for (uint8_t i = 0; i < 12; i++)
            {
                ws2812_write(ws2812_handle, i, 10, 10, 10);
            }
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

#if 0 // 使用的是set_servo_angle函数
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

            set_servo_angle(2, angle);      
            set_servo_angle(1, 30 - angle); 
            vTaskDelay(pdMS_TO_TICKS(50));  // 5ms延时
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

#elif 0 // 使用的是servo_smooth_move函数
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
    servo_smooth_move(0, 100, 0, 2);
    while (current_time <= 4.5)
    {
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
        vTaskDelay(pdMS_TO_TICKS(70));
        current_time += step;
    }

    // 将头部舵机转到最佳角度
    // set_servo_angle(0, best_angle);
    servo_smooth_move(0, 100, best_angle, 2);
}

#elif 1 // 使用servo_smooth_move函数+距离获取任务，同时需要将smooth_flag改为1
void shake()
{
    float hcsrdata;                // 声明hcsrmsg结构体变量
    uint8_t count1 = 0;            // 手臂舵机计数
    bool flip_flag = false;        // 手臂舵机反转标志位
    float best_distance = FLT_MAX; // 初始化为最大值
    uint8_t best_angle = 0;        // 存储最佳角度
    double step = 0.05;            // 每个插值点的时间间隔
    double current_time = 0;       // 当前时间
    double target_angle = 100;     // 目标位置
    servo_smooth_move(0, 0, 1);    // 转动回0位
    while (current_time <= 6)      // 当前时间小于6
    {
        double angle = 0 + ((target_angle - 0) * (current_time / 6));
        set_servo_angle(0, angle);
        count1++;
        if (xQueueReceive(distanceQueue, &hcsrdata, 0) == pdPASS)
        { // 判断接收来自距离检测任务的数据
            if (hcsrdata >= 0 && hcsrdata < best_distance) 
            {//如果检测到的距离在正确范围内
                best_distance = hcsrdata;      //赋值给最优距离
                best_angle = angle;            //赋值给最优角度
            }
        }
        if (count1 <= 30)       
        {//时间小于30
            uint8_t arm_angle = flip_flag ? count1 : (30 - count1);
            // set_servo_angle(2, arm_angle);
            // set_servo_angle(1, 30 - arm_angle);
            servo_smooth_move(2,arm_angle,1);           //使用舵机平滑处理函数
            servo_smooth_move(2,30 - arm_angle,1);      //使用舵机平滑处理函数
        }
        else                   
        {//时间到达30反转标志位
            count1 = 0;        //从0计数
            flip_flag = !flip_flag;
        }
        vTaskDelay(pdMS_TO_TICKS(70));
        current_time += step; //步进累加
    }
    servo_smooth_move(0, best_angle, 1);// 将头部舵机转到最佳角度
}
#endif

//*******************************suspend resume task*********************************** */
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
                end_flag = true;
                task_control_flag = true;
                ESP_LOGI("msgtype", "1");
                break;
            case 2:
                Audio_init(2, voice_volume);
                A_choose(2);
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
                A_choose(msg.msgtype - 2);
                vTaskDelay(pdMS_TO_TICKS(1000));
                end_flag = false;
                task_control_flag = true;
                break;
            case 15:
            case 16:
                num = 9; // 灯光全亮状态
                task_control_flag = false;
                vTaskDelay(pdMS_TO_TICKS(1000));
                A_stop();
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
    xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);
    xTaskCreate(psychic_run_task, "psychic_run_task", 4096, NULL, 5, NULL);
    xTaskCreate(checkplystation, "checkplystation", 4096, NULL, 5, &check_task_handle);
    xTaskCreate(Get_Distance_task, "Get_Distance_task", 4096, NULL, 5, NULL);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ws2812_deinit(ws2812_handle);
}
