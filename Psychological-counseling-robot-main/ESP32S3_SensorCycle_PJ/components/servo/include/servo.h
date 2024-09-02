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

// 初始化单个舵机
void servo_init(int channel, int gpio_num);

// 设置舵机角度
void set_servo_angle(int channel, int angle);


#endif







