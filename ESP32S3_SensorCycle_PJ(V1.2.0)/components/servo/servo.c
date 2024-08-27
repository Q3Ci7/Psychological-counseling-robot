#include "servo.h"


static const char *TAG = "ServoControl";

// 初始化单个舵机
void servo_init(int channel, int gpio_num) {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 分辨率为13位
        .freq_hz = 50,                        // 频率为50Hz
        .speed_mode = LEDC_LOW_SPEED_MODE,    // 使用低速模式
        .timer_num = LEDC_TIMER_0             // 使用定时器0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel    = channel,
        .duty       = 0,
        .gpio_num   = gpio_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    };
    ledc_channel_config(&ledc_channel);
}

// 设置舵机角度
void set_servo_angle(int channel, int angle) {
    if (angle < 0) angle = 0;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;
    
    int pulsewidth = SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_ANGLE);
    int duty = (pulsewidth * (1 << 13)) / 20000; // 转换为13位占空比
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}
