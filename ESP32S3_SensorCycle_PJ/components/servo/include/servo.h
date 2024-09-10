#ifndef _SERVO_H
#define _SERVO_H

#include "common.h"



/********************************************************** */
// ����������С����������Լ����Ƕ�
#define SERVO_MIN_PULSEWIDTH 500
#define SERVO_MAX_PULSEWIDTH 2500
#define SERVO_MAX_ANGLE 180

// �������֧�ֵĶ������
#define MAX_SERVOS 8


#define smooth_flag  0//��ѡ ��ʼλ��-Ŀ��λ��/ֱ�ӵ�Ŀ��λ��

// ��ʼ���������
void servo_init(int channel, int gpio_num);

// ���ö���Ƕ�

void set_servo_angle(int channel, int angle);

#if smooth_flag
esp_err_t servo_smooth_move(int pwm_channel,double start_angle,double target_angle, double duration);
#else
esp_err_t servo_smooth_move(int pwm_channel, double target_angle, double duration);
#endif


#endif







