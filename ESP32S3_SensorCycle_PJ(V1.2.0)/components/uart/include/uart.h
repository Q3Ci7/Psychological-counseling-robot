#ifndef UART_H
#define UART_H

#include "common.h"


#define BUF_SIZE 1024


// ����һ�������������洢����Ҫ���ݵ�����
typedef struct {
    int emotype;
} Message;

extern QueueHandle_t data_queue2;

void init_uart();
void rx_uart_task(void *pvParameters);

#endif
