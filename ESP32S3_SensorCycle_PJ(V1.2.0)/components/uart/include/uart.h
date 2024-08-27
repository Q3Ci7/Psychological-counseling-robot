#ifndef UART_H
#define UART_H

#include "common.h"


#define BUF_SIZE 1024


// 定义一个数据类型来存储您想要传递的数据
typedef struct {
    int emotype;
} Message;

extern QueueHandle_t data_queue2;

void init_uart();
void rx_uart_task(void *pvParameters);

#endif
