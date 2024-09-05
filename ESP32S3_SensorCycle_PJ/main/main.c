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
const uint8_t voice_volume = 30;
const uint8_t duration = 2;

/************WS2812 Setting************/
uint8_t num = 0; // light mode choose
ws2812_strip_handle_t ws2812_handle = NULL;
int idx = 0;         //
uint8_t lednum = 12; // Number of WS2812B_LED

// QueueHandle_t data_queue1; // Create queue handle
QueueHandle_t data_queue2;                    // Create queue handle
QueueHandle_t data_queue3;                    // Create queue handle
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

void shake()
{
    uint8_t count1 = 0;
    uint8_t count2 = 0;
    bool flip_flag = false;
    float distance;
    float best_distance = FLT_MAX; // ��ʼ��Ϊ���ֵ
    uint8_t best_angle = 0;        // �洢��ѽǶ�

    for (uint8_t i = 0; i <= 120; i++)
    {
        count1++;
        count2 += 5;

        if (count1 <= 30)
        {
            uint8_t angle = flip_flag ? count1 : (30 - count1);

            set_servo_angle(2, angle);      // �ֲ����
            set_servo_angle(1, 30 - angle); // ͷ�����
            vTaskDelay(pdMS_TO_TICKS(50));  // 5ms��ʱ
        }
        else
        {
            count1 = 0;
            flip_flag = !flip_flag; // ��ת��־λ
        }

        // ÿ5������һ���������λ��
        if (i % 5 == 0)
        {
            set_servo_angle(0, i);          // ����ͷ������Ƕ�
            vTaskDelay(pdMS_TO_TICKS(100)); // �����ӳ٣��ӿ��ٶ�

            if (i != 0)
            {
                distance = ultrasonic_get_distance(); // ��ȡ��ǰ�Ƕȵľ���
                // printf("distance:%.2fcm\n", distance);
                // ��¼��С���뼰���Ӧ�ĽǶ�
                if (distance >= 0 && distance < best_distance)
                {
                    best_distance = distance;
                    best_angle = i;
                }
            }
        }
    }

    // ��ͷ�����ת����ѽǶ�
    set_servo_angle(0, best_angle);
}

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
                end_flag = true; // �������������������л���־λ���л������͡�selfend��
                task_control_flag = true;//������������״̬�������
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
                num = 8;//�ƹ�ȫ��״̬
                Audio_init(2, music_volume);
                A_choose(msg.msgtype - 7);
                vTaskDelay(pdMS_TO_TICKS(1000));
                end_flag = false; // �������������������л���־λ���л������͡�playend��
                task_control_flag = true;//������������״̬�������
                ESP_LOGI("msgtype", "%d", msg.msgtype);
                break;
            case 14:
                num = 9;//�ƹ�ȫ��״̬
                task_control_flag = false;
                vTaskDelay(pdMS_TO_TICKS(1000));
                A_stop();
                ESP_LOGI("msgtype", "14");
                break;
            case 15:
                num = 9;//�ƹ�ȫ��״̬
                task_control_flag = false;
                vTaskDelay(pdMS_TO_TICKS(1000));
                A_stop();
                // ESP_LOGI("msgtype", "15");
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
    set_servo_angle(0, 60); // Initialize the header Angle
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

    /**************Create a control semaphore**************/
    // control_semaphore = xSemaphoreCreateBinary();
    // if (control_semaphore == NULL)
    // {
    //     ESP_LOGE("app_main", "Failed to create semaphore");
    //     return;
    // }

    /**************CREATE TASK**************/
    xTaskCreate(lightmode_task, "lightmode_task", 4096, NULL, 5, NULL);
    xTaskCreate(rx_uart_task, "rx_uart_task", 4096, NULL, 5, NULL);
    xTaskCreate(psychic_run_task, "psychic_run_task", 4096, NULL, 5, NULL);
    xTaskCreate(checkplystation, "checkplystation", 4096, NULL, 5, &check_task_handle);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ws2812_deinit(ws2812_handle);
}
