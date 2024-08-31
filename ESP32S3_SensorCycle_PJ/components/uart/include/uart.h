#ifndef UART_H
#define UART_H

#include "common.h"

#define BUF_SIZE 1024

// ����һ�������������洢����Ҫ���ݵ�����
typedef struct
{
    uint8_t emotype;
    // uint8_t voicetype;
    // uint8_t musictype;
    uint8_t msgtype;
} Message;

extern QueueHandle_t data_queue2;

void init_uart();
void rx_uart_task(void *pvParameters);

#endif
