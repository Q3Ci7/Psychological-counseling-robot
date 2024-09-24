#ifndef UART_H
#define UART_H

#include "common.h"

#define BUF_SIZE 1024

// 定义一个数据类型来存储您想要传递的数据
typedef struct
{
    // uint8_t emotype;
    // uint8_t voicetype;
    // uint8_t musictype;
    uint8_t msgtype;
} Message;


typedef struct
{
    uint8_t emotype;
}EMO;

extern QueueHandle_t data_queue2;
extern QueueHandle_t data_queue3;


extern volatile bool shake_flag; 
extern volatile bool sdbk_flag;

void init_uart();
void rx_uart_task(void *pvParameters);
void uart_event_task(void *pvParameters);

#endif
