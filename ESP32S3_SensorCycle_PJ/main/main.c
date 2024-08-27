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

/************************/
// // Set weight value
// #define W1 0.7
// #define W2 0.3
// //
// typedef struct
// {
//     float Angry;
//     float Disgust;
//     float Fear;
//     float Happy;
//     float Sad;
//     float Surprise;
//     float Neutral;
// } EmotionProb;

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

// void sensorCycle_task(void *pvParameters)
// {
//     bpm_spo2_t data;
//     Message msg;
//     bool first_Receive_Data = false;  //first data reception flag bit
//     bool music_1_playing = false; // normal emotion music flag
//     bool music_2_playing = false; // Positive  emotion music flag
//     bool music_3_playing = false; // negative emotion music flag
//     float start_bpm = 0;          // Record initial heart rate
//     float growth_rate = 0;        // Record each growth rate
//     EmotionProb Hprob;            // Declare the heart rate probability struct variable.
//     EmotionProb Fprob;            // Declare the face probability struct variable.
//     EmotionProb Zprob;            // Declare the synthetic probability struct variable.
//     static char *send_data;       // postback data
//     while (1)
//     {
//         // Determine if queue 1 has received data and calculate the added value
//         if (xQueueReceive(data_queue1, &data, 0) == pdPASS)
//         {
//             if (!first_Receive_Data)
//             {
//                 start_bpm = data.heart_bpm;
//                 ESP_LOGI("sensorCycle_task:", "The first received heart rate data: %.2f has been stored", start_bpm);
//                 first_Receive_Data = true;
//             }
//             ESP_LOGI("AVG Heart BPM:", "%.2f", data.heart_bpm);
//             growth_rate = ((float)(data.heart_bpm - start_bpm) / start_bpm); // Calculate growth value
//             ESP_LOGI("growth_rate:", "%f", growth_rate);
//
//             //***********************Angry|Disgust|Fear|Happy|Sad|Surprise|Neutral***********************/
//             if (growth_rate < 0)
//             {
//                 Hprob = (EmotionProb){0.1, 0.3, 0.1, 0.1, 0.3, 0.05, 0.05}; // Tend to be negative or depressed.||such as: Sad, Disgust
//             }
//             else if (growth_rate <= 5)
//             {
//                 Hprob = (EmotionProb){0.1, 0.1, 0.1, 0.4, 0.1, 0.1, 0.4}; // Tend to neutral or pleasant emotions.|| such as: neutral and happy
//             }
//             else
//             {
//                 Hprob = (EmotionProb){0.3, 0.1, 0.3, 0.1, 0.05, 0.3, 0.05}; // Tend to be nervous or excited.||such as: Angry, Fear, Surprise
//             }
//         }
//
//         // Determine if queue 2 has received data and calculate the final sentiment
//         if (xQueueReceive(data_queue2, &msg, 0) == pdPASS)
//         {
//             memset(&Fprob, 0, sizeof(Fprob)); // All members of the "Fprob" variable are initialized to 0
//             memset(&Hprob, 0, sizeof(Hprob)); // All members of the "Hprob" variable are initialized to 0
//             ESP_LOGI("Received EMO:", "%d", msg.emotype);
//             switch (msg.emotype)
//             {
//             case 1:
//                 Fprob.Surprise = 1.0;
//                 break;
//             case 2:
//                 Fprob.Happy = 1.0;
//                 break;
//             case 3:
//                 Fprob.Neutral = 1.0;
//                 break;
//             case 4:
//                 Fprob.Sad = 1.0;
//                 break;
//             case 5:
//                 Fprob.Disgust = 1.0;
//                 break;
//             case 6:
//                 Fprob.Fear = 1.0;
//                 break;
//             case 7:
//                 Fprob.Angry = 1.0;
//                 break;
//             case 8:
//                 Audio_init(2, 25);
//                 A_choose(4);
//                 send_data = "plydone";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 music_1_playing = false;
//                 music_2_playing = false;
//                 music_3_playing = false;
//                 break;
//             default:
//                 break;
//             }
//             // Calculate weighted averages for each emotion using W1 and W2
//             Zprob.Angry = W1 * Fprob.Angry + W2 * Hprob.Angry;
//             Zprob.Disgust = W1 * Fprob.Disgust + W2 * Hprob.Disgust;
//             Zprob.Fear = W1 * Fprob.Fear + W2 * Hprob.Fear;
//             Zprob.Happy = W1 * Fprob.Happy + W2 * Hprob.Happy;
//             Zprob.Sad = W1 * Fprob.Sad + W2 * Hprob.Sad;
//             Zprob.Surprise = W1 * Fprob.Surprise + W2 * Hprob.Surprise;
//             Zprob.Neutral = W1 * Fprob.Neutral + W2 * Hprob.Neutral;
//
//             // Calculate the maximum value
//             float values[] = {Zprob.Angry, Zprob.Disgust, Zprob.Fear, Zprob.Happy, Zprob.Sad, Zprob.Surprise, Zprob.Neutral};
//             uint8_t max_index = 7;
//             for (int i = 0; i < 7; ++i)
//             {
//                 if (values[i] > values[max_index])
//                 {
//                     max_index = i;
//                 }
//             }
//
//             // Perform actions based on emotional results
//             switch (max_index)
//             {
//             case 0:
//                 ESP_LOGI("final emo", "angry");
//                 ws2812_write(ws2812_handle, idx, 250, 0, 0); // Angry -- red
//                 if (!music_3_playing)
//                 {
//                     Audio_init(1, 30);
//                     A_choose(3);
//                     music_1_playing = false;
//                     music_2_playing = false;
//                     music_3_playing = true;
//                 }
//                 warm_stop();
//                 send_data = "angry";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 1:
//                 ESP_LOGI("final emo", "disgust");
//                 ws2812_write(ws2812_handle, idx, 0, 250, 0); // Disgust -- green
//                 if (!music_3_playing)
//                 {
//                     Audio_init(1, 30);
//                     A_choose(3);
//                     music_1_playing = false;
//                     music_2_playing = false;
//                     music_3_playing = true;
//                 }
//                 warm_stop();
//                 send_data = "disgust";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 2:
//                 ESP_LOGI("final emo", "fear");
//                 ws2812_write(ws2812_handle, idx, 0, 0, 128); // Fear -- Navy blue
//                 if (!music_3_playing)
//                 {
//                     Audio_init(1, 30);
//                     A_choose(3);
//                     music_1_playing = false;
//                     music_2_playing = false;
//                     music_3_playing = true;
//                 }
//                 warm_begin();
//                 send_data = "fear";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 3:
//                 ESP_LOGI("final emo", "happy");
//                 ws2812_write(ws2812_handle, idx, 255, 255, 0); // Happy -- yellow
//                 if (!music_2_playing)
//                 {
//                     Audio_init(1, 25);
//                     A_choose(2);
//                     music_1_playing = false;
//                     music_2_playing = true;
//                     music_3_playing = false;
//                 }
//                 warm_stop();
//                 send_data = "happy";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 4:
//                 ESP_LOGI("final emo", "Sad");
//                 ws2812_write(ws2812_handle, idx, 135, 206, 235); // Sad -- light blue
//                 if (!music_3_playing)
//                 {
//                     Audio_init(1, 30);
//                     A_choose(3);
//                     music_1_playing = false;
//                     music_2_playing = false;
//                     music_3_playing = true;
//                 }
//                 warm_begin();
//                 send_data = "sad";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 5:
//                 ESP_LOGI("final emo", "surprise");
//                 ws2812_write(ws2812_handle, idx, 128, 0, 128); // Surprise -- purple
//                 if (!music_2_playing)
//                 {
//                     Audio_init(1, 25);
//                     A_choose(2);
//                     music_1_playing = false;
//                     music_2_playing = true;
//                     music_3_playing = false;
//                 }
//                 warm_stop();
//                 send_data = "surprise";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             case 6:
//                 ESP_LOGI("final emo", "Neutral");
//                 ws2812_write(ws2812_handle, idx, 192, 192, 192); // Neutral -- grey
//                 if (!music_1_playing)
//                 {
//                     Audio_init(1, 25);
//                     A_choose(1);
//                     music_1_playing = true;
//                     music_2_playing = false;
//                     music_3_playing = false;
//                 }
//                 warm_stop();
//                 send_data = "neutral";
//                 uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
//                 break;
//             default:
//                 break;
//             }
//         }
//
//         vTaskDelay(pdMS_TO_TICKS(1000));
//     }
// }

void sensorCycle_task(void *pvParameters)
{
    bpm_spo2_t data;
    Message msg;
    bool first_Receive_Data = false;
    bool music_1_playing = false;
    bool music_2_playing = false;
    bool music_3_playing = false;
    float start_bpm = 0;
    float growth_rate = 0;
    EmotionProb Hprob;
    EmotionProb Fprob;
    EmotionProb Zprob;

    while (1)
    {
        if (xQueueReceive(data_queue1, &data, 0) == pdPASS)
        {
            processHeartRateData(data, &Hprob, &first_Receive_Data, &start_bpm, &growth_rate);
        }

        if (xQueueReceive(data_queue2, &msg, 0) == pdPASS)
        {
            processFacialEmotionData(msg, &Fprob, &Hprob, &Zprob);
            calculateFinalEmotionProbabilities(&Fprob, &Hprob, &Zprob);
            performActionsBasedOnEmotion(&Zprob, &music_1_playing, &music_2_playing, &music_3_playing);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
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

    /************JUST FOR TSET**********/
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
