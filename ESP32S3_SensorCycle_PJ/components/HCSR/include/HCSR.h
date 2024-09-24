#ifndef _HCSR_H
#define _HCSR_H

#include "common.h"

// ���ó�����������������
#define TRIGGER_PIN  GPIO_NUM_4
#define ECHO_PIN     GPIO_NUM_5

// float hcsrdata;

extern QueueHandle_t distanceQueue;//


// ��ʼ��������ģ��
void ultrasonic_init(void);
void ultrasonic_deinit(void);
// ������ಢ���ؾ��루��λ�����ף�
float ultrasonic_get_distance(void);
// ������ಢ���ؾ��루��λ�����ף�
void Get_Distance_task(void *pvParameters);


#endif 
