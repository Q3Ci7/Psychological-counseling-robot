#include "uart.h"
#include "servo.h"
QueueHandle_t uart_queue;

/** 串口初始化
 * @param uart_num 串口号配置
 * @param baud_rate 波特率配置
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

    // uart_driver_install(UART_NUM_2, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART_NUM_2, &uart_config);
    uart_set_pin(UART_NUM_2, 11, 12, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // 创建一个 UART 事件队列
    uart_driver_install(UART_NUM_2, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart_queue, 0);
}

// 函数化处理消息发送
void send_message_to_queue(int queue_type, void *msg, int msg_size)
{
    if (xQueueSend(queue_type == 2 ? data_queue2 : data_queue3, msg, portMAX_DELAY) != pdPASS)
    {
        ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
    }
}

// 字符串和消息类型对应表
typedef struct
{
    char *str;
    int msgtype;
    int queue_type;
} CommandMapping;

CommandMapping commands[] = {
    {"self", 1, 2}, {"pic", 2, 2}, {"warm", 1, 3}, {"happy", 2, 3}, {"calm", 3, 3}, {"sad", 4, 3}, {"afraid", 5, 3}, {"scared", 6, 3}, {"anger", 7, 3},
     {"1zh", 3, 2}, {"2zh", 4, 2}, {"3zh", 5, 2}, {"4zh", 6, 2}, 
     {"1en", 7, 2}, {"2en", 8, 2}, {"3en", 9, 2}, {"4en", 10, 2}, 
     {"1it", 11, 2}, {"2it", 12, 2}, {"3it", 13, 2}, {"4it", 14, 2}, 
     {"stopplay", 15, 2}, {"end", 16, 2}};

// UART事件处理任务
void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    Message msg;
    EMO emo;
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);

    while (1)
    {
        // 等待UART事件
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(data, BUF_SIZE);

            // 处理接收到的数据
            if (event.type == UART_DATA)
            {
                int len = uart_read_bytes(UART_NUM_2, data, event.size, portMAX_DELAY);
                data[len] = '\0'; // 添加字符串终止符

                bool matched = false;

                // 遍历命令表进行匹配
                for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++)
                {
                    if (strstr((char *)data, commands[i].str))
                    {
                        // 如果接收到 "end" 字符串，停止 shake 动作
                        if (strcmp(commands[i].str, "end") == 0)
                        {
                            shake_flag = false; // 修改 shake_flag 停止 shake
                            sdbk_flag = false; 
                            set_servo_angle(0, 52);
                            set_servo_angle(2, 0);
                            set_servo_angle(1, 30);
                            ESP_LOGI("UART", "Received 'end', shake_flag set to false");
                        }
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
                        break; // 匹配成功后，停止继续匹配
                    }
                }

                if (!matched)
                {
                    ESP_LOGI("xQueueSend error:", "Unknown emotion");
                }
            }
        }
    }
    free(data);
}
