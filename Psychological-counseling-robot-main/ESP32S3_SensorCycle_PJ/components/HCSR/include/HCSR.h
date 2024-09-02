#ifndef _HCSR_H
#define _HCSR_H

#include "common.h"

// 配置超声波传感器的引脚
#define TRIGGER_PIN  GPIO_NUM_4
#define ECHO_PIN     GPIO_NUM_5

// 初始化超声波模块
void ultrasonic_init(void);

// 触发测距并返回距离（单位：厘米）
float ultrasonic_get_distance(void);

#endif // ULTRASONIC_H
