#ifndef UART_H
#define UART_H

#include "common.h"

#define BUF_SIZE 1024

<<<<<<< HEAD
// 定义一个数据类型来存储您想要传递的数据
=======
// ����һ�������������洢����Ҫ���ݵ�����
>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
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
<<<<<<< HEAD


=======
>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
extern volatile bool shake_flag; 

void init_uart();
void rx_uart_task(void *pvParameters);
<<<<<<< HEAD
void uart_event_task(void *pvParameters);

=======
void setup_uart_pattern_intr();
>>>>>>> 0e1be01253a481eeee4cd4c9f11d3abc9f9e8aed
#endif
