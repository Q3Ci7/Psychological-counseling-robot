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

    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, 11, 12, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

/** 串口接收数据
 * @param pvParameters
 */
void rx_uart_task(void *pvParameters)
{
    static Message msg;
    msg.emotype = 0;
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1)
    {
        int len = uart_read_bytes(UART_NUM_2, (uint8_t *)data, BUF_SIZE - 1, 20 / portTICK_PERIOD_MS);
        if (len > 0)
        {
            data[len] = '\0'; // Add "null" at the end of the sentence

            if (strstr((char *)data, "surprise"))
            {
                msg.emotype = 1;
            }
            else if (strstr((char *)data, "happy"))
            {
                msg.emotype = 2;
            }
            else if (strstr((char *)data, "neutral"))
            {
                msg.emotype = 3;
            }
            else if (strstr((char *)data, "sad"))
            {
                msg.emotype = 4;
            }
            else if (strstr((char *)data, "disgust"))
            {
                msg.emotype = 5;
            }
            else if (strstr((char *)data, "fear"))
            {
                msg.emotype = 6;
            }
            else if (strstr((char *)data, "angry"))
            {
                msg.emotype = 7;
            }
            else if (strstr((char *)data, "begin") || strstr((char *)data, "end"))
            {
                msg.emotype = 8;
            }
            // else if (strstr((char *)data, "CALM"))
            // {
            //     msg.emotype = 7;
            // }
            // else if (strstr((char *)data, "POSITIVE"))
            // {
            //     msg.emotype = 7;
            // }
            // else if (strstr((char *)data, "NEGTIVE"))
            // {
            //     msg.emotype = 7;
            // }
            else
            {
                msg.emotype = -1; // unkown emotions
                ESP_LOGI("xQueueSend error:", "unknown emotions");
            }

            if (xQueueSend(data_queue2, &msg, portMAX_DELAY) != pdPASS)
            {
                ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    free(data);
}