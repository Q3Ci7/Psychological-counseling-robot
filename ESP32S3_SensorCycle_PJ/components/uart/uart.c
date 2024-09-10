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
            else if (strstr((char *)data, "1"))
            {
                msg.msgtype = 3;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "2"))
            {
                msg.msgtype = 4;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "3"))
            {
                msg.msgtype = 5;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "4"))
            {
                msg.msgtype = 6;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "5"))
            {
                msg.msgtype = 7;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "6"))
            {
                msg.msgtype = 8;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "7"))
            {
                msg.msgtype = 9;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "8"))
            {
                msg.msgtype = 10;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }//===============================================================================================
            else if (strstr((char *)data, "9"))
            {
                msg.msgtype = 11;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "10"))
            {
                msg.msgtype = 12;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "11"))
            {
                msg.msgtype = 13;
                if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
            }
            else if (strstr((char *)data, "12"))
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

            // if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
            // {
            //     ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
            // }
            // if (xQueueSend(data_queue3, &emo, portMAX_DELAY) != pdPASS)
            // {
            //     ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
            // }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    free(data);
}
