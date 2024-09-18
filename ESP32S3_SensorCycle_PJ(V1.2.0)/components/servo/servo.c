#include "servo.h"


static const char *TAG = "ServoControl";

// ��ʼ���������
void servo_init(int channel, int gpio_num) {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // �ֱ���Ϊ13λ
        .freq_hz = 50,                        // Ƶ��Ϊ50Hz
        .speed_mode = LEDC_LOW_SPEED_MODE,    // ʹ�õ���ģʽ
        .timer_num = LEDC_TIMER_0             // ʹ�ö�ʱ��0
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

// ���ö���Ƕ�
void set_servo_angle(int channel, int angle) {
    if (angle < 0) angle = 0;
    if (angle > SERVO_MAX_ANGLE) angle = SERVO_MAX_ANGLE;
    
    int pulsewidth = SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_ANGLE);
    int duty = (pulsewidth * (1 << 13)) / 20000; // ת��Ϊ13λռ�ձ�
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}
