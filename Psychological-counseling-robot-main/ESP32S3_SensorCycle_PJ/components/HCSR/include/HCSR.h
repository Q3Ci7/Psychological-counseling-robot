#ifndef _HCSR_H
#define _HCSR_H

#include "common.h"

// ���ó�����������������
#define TRIGGER_PIN  GPIO_NUM_4
#define ECHO_PIN     GPIO_NUM_5

// ��ʼ��������ģ��
void ultrasonic_init(void);

// ������ಢ���ؾ��루��λ�����ף�
float ultrasonic_get_distance(void);

#endif // ULTRASONIC_H
