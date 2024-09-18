#ifndef BPM_H
#define BPM_H

#include "common.h"

#define I2C_SDA 9      
#define I2C_SCL 10     
#define I2C_FRQ 100000
#define I2C_PORT I2C_NUM_0

// ����һ�������������洢����Ҫ���ݵ�����
typedef struct
{
    float heart_bpm;
    //     float spO2;
} bpm_spo2_t;

extern QueueHandle_t data_queue1;

void i2c_init();
void get_bpm_task(void *param);

#endif
