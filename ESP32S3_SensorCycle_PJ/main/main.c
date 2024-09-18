<<<<<<< HEAD
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
volatile bool shake_flag = false; // ç”¨äºŽæŽ§åˆ¶

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
    float best_distance = FLT_MAX; // åˆå§‹åŒ–ä¸ºæœ€å¤§å€¼
    uint8_t best_angle = 0;        // å­˜å‚¨æœ€ä½³è§’åº¦
    double step = 0.05;            // æ¯ä¸ªæ’å€¼ç‚¹çš„æ—¶é—´é—´éš”
    double current_time = 0;
    double target_angle = 100;
    // servo_smooth_move(0, 52, 0, 2);
    while (current_time <= 4.5 && shake_flag)
    {
        ESP_LOGI("current_time", "%.2f",current_time);//æ—¥å¿—æ‰“å°shakeå‡½æ•°è¿è¡ŒçŠ¶æ€
        double angle = 0 + ((target_angle - 0) * (current_time / 4.5));
        set_servo_angle(0, angle);
        count1++;

        distance = ultrasonic_get_distance(); // èŽ·å–å½“å‰è§’åº¦çš„è·ç¦»
        // printf("distance:%.2fcm\n", distance);
        if (distance >= 0 && distance < best_distance) // è®°å½•æœ€å°è·ç¦»åŠå…¶å¯¹åº”çš„è§’åº¦
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
            flip_flag = !flip_flag; // ç¿»è½¬æ ‡å¿—ä½
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        current_time += step;
    }

    // å°†å¤´éƒ¨èˆµæœºè½¬åˆ°æœ€ä½³è§’åº¦
    servo_smooth_move(0, 100, best_angle, 2);
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

                Audio_init(2, voice_volume);
                A_choose(1);
                servo_smooth_move(0, 52, 0, 2);//è½¬åŠ¨è‡³é›¶ä½
                shake_flag = true; //ä½¿èƒ½shake
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
                num = 8; // ç¯å…‰å…¨ç­çŠ¶æ€
                Audio_init(2, music_volume);
                A_choose(msg.msgtype);
                end_flag = 3;
                task_control_flag = true;
                ESP_LOGI("msgtype", "audiotype");
                break;
            case 15:
            case 16:
                num = 9; // ç¯å…‰å…¨äº®çŠ¶æ€
                task_control_flag = false;
                A_stop();
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
=======
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
volatile bool shake_flag = false; // ÓÃÓÚ¿ØÖÆ

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
            lighton();
            break;
        default:
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}


// void IRAM_ATTR interrupt_service_1(void)
// {
//     shake_flag = true;
//     ESP_LOGI("inshake_flag", "1");
// }

// void IRAM_ATTR interrupt_service_2(void)
// {
//     shake_flag = false;
//     ESP_LOGI("inshake_flag", "0");
// }


#if 1
void shake()
{
    uint8_t count1 = 0;
    bool flip_flag = false;
    float distance;
    float best_distance = FLT_MAX; // ³õÊ¼»¯Îª×î´óÖµ
    uint8_t best_angle = 0;        // ´æ´¢×î¼Ñ½Ç¶È
    double step = 0.05;            // Ã¿¸ö²åÖµµãµÄÊ±¼ä¼ä¸ô
    double current_time = 0;
    double target_angle = 100;
    servo_smooth_move(0, 52, 0, 2);
    while (current_time <= 4.5 && shake_flag)
    {
        double angle = 0 + ((target_angle - 0) * (current_time / 4.5));
        set_servo_angle(0, angle);
        count1++;

        distance = ultrasonic_get_distance(); // »ñÈ¡µ±Ç°½Ç¶ÈµÄ¾àÀë
        // printf("distance:%.2fcm\n", distance);
        if (distance >= 0 && distance < best_distance) // ¼ÇÂ¼×îÐ¡¾àÀë¼°Æä¶ÔÓ¦µÄ½Ç¶È
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
            flip_flag = !flip_flag; // ·­×ª±êÖ¾Î»
        }
        vTaskDelay(pdMS_TO_TICKS(50));
        current_time += step;
    }

    // ½«Í·²¿¶æ»ú×ªµ½×î¼Ñ½Ç¶È
    servo_smooth_move(0, 100, best_angle, 2);
    shake_flag = false;
}
#else
void shake_task(void *pvParameters)
{
    uint8_t count1 = 0;
    bool flip_flag = false;
    float distance;
    float best_distance = FLT_MAX; // ³õÊ¼»¯Îª×î´óÖµ
    uint8_t best_angle = 0;        // ´æ´¢×î¼Ñ½Ç¶È
    double step = 0.05;            // Ã¿¸ö²åÖµµãµÄÊ±¼ä¼ä¸ô
    double current_time = 0;
    double target_angle = 100;

    while (1)
    {
        if (shake_flag)
        {
            // Ö´ÐÐÒ¡°ÚÂß¼­
            while (current_time <= 4.5)
            {
                double angle = 0 + ((target_angle - 0) * (current_time / 4.5));
                set_servo_angle(0, angle);
                count1++;

                distance = ultrasonic_get_distance(); // »ñÈ¡µ±Ç°½Ç¶ÈµÄ¾àÀë
                if (distance >= 0 && distance < best_distance) // ¼ÇÂ¼×îÐ¡¾àÀë¼°Æä¶ÔÓ¦µÄ½Ç¶È
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
                    flip_flag = !flip_flag; // ·­×ª±êÖ¾Î»
                }
                vTaskDelay(pdMS_TO_TICKS(70));
                current_time += step;
            }
            // ½«Í·²¿¶æ»ú×ªµ½×î¼Ñ½Ç¶È
            servo_smooth_move(0, 100, best_angle, 2);
            shake_flag = false;
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(1000)); // Ã»ÓÐ±êÖ¾Î»Ê±£¬ÈÎÎñÐÝÃß
        }
    }
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
                // A_choose(1);
                // servo_smooth_move(0, 100, 0, 2);
                shake_flag = true;
                shake();
                end_flag = 0;
                task_control_flag = true;
                ESP_LOGI("msgtype", "1");
                break;
            case 2:
                Audio_init(2, voice_volume);
                A_choose(2);
                // shake_flag = false;
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
                num = 8; // µÆ¹âÈ«Ãð×´Ì¬
                Audio_init(2, music_volume);
                A_choose(msg.msgtype);
                vTaskDelay(pdMS_TO_TICKS(1000));
                // shake_flag = false;
                end_flag = 3;
                task_control_flag = true;
                break;
            case 15:
            case 16:
                num = 9; // µÆ¹âÈ«ÁÁ×´Ì¬
                // shake_flag = false;
                task_control_flag = false;
                vTaskDelay(pdMS_TO_TICKS(1000));
                A_stop();
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
    setup_uart_pattern_intr();
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
    // xTaskCreate(shake_task, "shake_task", 4096, NULL, 3, NULL);
    // xTaskCreate(Get_Distance_task, "Get_Distance_task", 4096, NULL, 5, NULL);

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    ws2812_deinit(ws2812_handle);
}
>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
