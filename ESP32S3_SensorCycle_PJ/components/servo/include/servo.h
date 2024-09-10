#ifndef _SERVO_H
#define _SERVO_H

#include "common.h"



/********************************************************** */
// 定义舵机的最小和最大脉宽，以及最大角度
#define SERVO_MIN_PULSEWIDTH 500
#define SERVO_MAX_PULSEWIDTH 2500
#define SERVO_MAX_ANGLE 180

// 定义最多支持的舵机数量
#define MAX_SERVOS 8


#define smooth_flag  0//可选 起始位置-目标位置/直接到目标位置

// 初始化单个舵机
void servo_init(int channel, int gpio_num);

// 设置舵机角度

void set_servo_angle(int channel, int angle);

#if smooth_flag
esp_err_t servo_smooth_move(int pwm_channel,double start_angle,double target_angle, double duration);
#else
esp_err_t servo_smooth_move(int pwm_channel, double target_angle, double duration);
#endif


#endif







