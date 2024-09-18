#include "uart.h"

/** 串口初�?�化
 * @param uart_num 串口号�?�置
 * @param baud_rate 波特率�?�置
 */
void init_uart()
{
    const uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    };

    uart_driver_install(UART_NUM_0, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM_1, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 16, 17, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, 11, 12, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}


void setup_uart_pattern_intr()
{
    uart_enable_pattern_det_baud_intr(UART_NUM_2, 0xFF, 10, 100, 0, 0); 
}


void IRAM_ATTR uart_isr_handler(void* arg)
{
    // uint16_t uart_num = (uint32_t) arg;
    uint8_t data[BUF_SIZE];
    int length = uart_read_bytes(UART_NUM_2, data, BUF_SIZE - 1, 100 / portTICK_PERIOD_MS);
    ESP_LOGI("UART_IAR:", "interrupt");
    if (length > 0) {
        data[length] = '\0'; // 确保数据以 NULL 结尾
        if (strcmp((char*)data, "end") == 0) {
            shake_flag = false; // 修改全局标志位
        }
    }
}


#if 0
/** 串口接收数据
 * @param pvParameters
 */
void rx_uart_task(void *pvParameters)
{
    Message msg;
    EMO emo;
    msg.msgtype = 0;
    emo.emotype = 0;
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_2, (uint8_t *)data, BUF_SIZE - 1, 20 / portTICK_PERIOD_MS);
        if (len > 0)
        {
            data[len] = '\0'; // Add "null" at the end of the sentence

            if (strstr((char *)data, "self"))
            {
                msg.msgtype = 1;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "pic"))
            {
                msg.msgtype = 2;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "warm"))
            {
                // msg.msgtype = 3;
                emo.emotype = 1;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "happy"))
            {
                // msg.msgtype = 4;
                emo.emotype = 2;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "calm"))
            {
                // msg.msgtype = 5;
                emo.emotype = 3;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "sad"))
            {
                // msg.msgtype = 6;
                emo.emotype = 4;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "afraid"))
            {
                // msg.msgtype = 7;
                emo.emotype = 5;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "scared"))
            {
                // msg.msgtype = 8;
                emo.emotype = 6;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "anger"))
            {
                // msg.msgtype = 9;
                emo.emotype = 7;
                if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "1zh"))
            {
                msg.msgtype = 3;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "2zh"))
            {
                msg.msgtype = 4;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "3zh"))
            {
                msg.msgtype = 5;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "4zh"))
            {
                msg.msgtype = 6;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "1en"))
            {
                msg.msgtype = 7;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "2en"))
            {
                msg.msgtype = 8;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "3en"))
            {
                msg.msgtype = 9;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "4en"))
            {
                msg.msgtype = 10;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "1it"))
            {
                msg.msgtype = 11;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "2it"))
            {
                msg.msgtype = 12;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "3it"))
            {
                msg.msgtype = 13;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "4it"))
            {
                msg.msgtype = 14;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "stopplay"))
            {
                msg.msgtype = 15;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "end"))
            {
                msg.msgtype = 16;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else
            {
                ESP_LOGI("xQueueSend error:", "unknown emotions");
            }

        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    free(data);
}

#elif 1

// 函数化处理消息发送
void send_message_to_queue(int queue_type, void *msg, int msg_size)
{
    if (xQueueSend(queue_type == 2 ? data_queue2 : data_queue3, msg, portMAX_DELAY) != pdPASS)
    {
        ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
    }
}

// 字符串和消息类型对应表
typedef struct {
    char *str;
    int msgtype;
    int queue_type;
} CommandMapping;

CommandMapping commands[] = {
    {"self", 1, 2}, {"pic", 2, 2}, {"warm", 1, 3}, {"happy", 2, 3}, {"calm", 3, 3},
    {"sad", 4, 3}, {"afraid", 5, 3}, {"scared", 6, 3}, {"anger", 7, 3}, 
    {"1zh", 3, 2}, {"2zh", 4, 2}, {"3zh", 5, 2}, {"4zh", 6, 2},
    {"1en", 7, 2}, {"2en", 8, 2}, {"3en", 9, 2}, {"4en", 10, 2},
    {"1it", 11, 2}, {"2it", 12, 2}, {"3it", 13, 2}, {"4it", 14, 2},
    {"stopplay", 15, 2}, {"end", 16, 2}
};

void rx_uart_task(void *pvParameters)
{
    Message msg;
    EMO emo;
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_2, (uint8_t *)data, BUF_SIZE - 1, 20 / portTICK_PERIOD_MS);
        if (len > 0)
        {
            data[len] = '\0'; // Add null terminator
            bool matched = false;

            // 遍历命令表进行匹配
            for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
            {
                if (strstr((char *)data, commands[i].str))
                {
                    if (commands[i].queue_type == 2)
                    {
                        msg.msgtype = commands[i].msgtype;
                        send_message_to_queue(2, &msg, sizeof(msg));
                    }
                    else
                    {
                        emo.emotype = commands[i].msgtype;
                        send_message_to_queue(3, &emo, sizeof(emo));
                    }
                    matched = true;
                    break; // 一旦匹配成功，停止继续匹配
                }
            }

            if (!matched)
            {
                ESP_LOGI("xQueueSend error:", "Unknown emotion");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    free(data); // 确保释放内存
}





#endif







