#include "servo.h"

/*
*
*
*
*
* * @brief ������Ƴ���
 * 
 * �ó���ʹ�� ESP32 �� LEDC��LED��������ģ�������ƶ���� PWM��������ƣ��źš�LEDC ģ���ܹ����ɸ߾��ȵ� PWM �źţ������ڶ�����ơ�
 * 
 * LEDC ģ�����õ���Ҫ�������£�
 * 
 * 1. **��ʼ�����**��
 *    - ���� LEDC ��ʱ�������� PWM ��Ƶ�ʺͷֱ��ʡ������ڿ��ƶ���� PWM �ź�Ƶ��Ϊ50Hz����׼���Ƶ�ʣ���
 *    - ���� LEDC ͨ�������� PWM �źŵ�������źͳ�ʼռ�ձȡ�ÿ��ͨ�����Կ���һ�������
 * 
 * 2. **���ö���Ƕ�**��
 *    - ͨ����Ŀ��Ƕ�ת��Ϊ��Ӧ�� PWM �������� PWM ��ռ�ձȣ��Ӷ����ƶ����λ�á�
 *    - ��������� PWM ռ�ձȣ�ʹ���ڶ���ɽ��ܵ�����Χ�ڣ�ͨ����500��s��2500��s֮�䣩��
 * 
 * ����˵����
 * 
 * - `servo_init(int channel, int gpio_num)`��
 *   ��ʼ��ָ��ͨ���� PWM ���ƣ��Կ��ƶ�����˶������� LEDC ��ʱ��������Ƶ�ʺͷֱ��ʣ��������� LEDC ͨ���� GPIO ���źͳ�ʼռ�ձȡ�
 * 
 * - `set_servo_angle(int channel, int angle)`��
 *   ���ݸ����ĽǶȼ��� PWM ���������� PWM ռ�ձ��Ե��������λ�á��Ƕ�ֵ����������Ч��Χ�ڣ�����ת��Ϊ 13 λ��ռ�ձȡ�
 *
****************************************************************************************************/
// ��ʼ���������
void servo_init(int channel, int gpio_num)
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // �ֱ���Ϊ13λ
        .freq_hz = 50,                        // Ƶ��Ϊ50Hz
        .speed_mode = LEDC_LOW_SPEED_MODE,    // ʹ�õ���ģʽ
        .timer_num = LEDC_TIMER_0             // ʹ�ö�ʱ��0
    };
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {
        .channel = (ledc_channel_t)channel,
        .duty = 0, 
        .gpio_num = gpio_num,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint = 0,
        .timer_sel = LEDC_TIMER_0};
    ledc_channel_config(&ledc_channel);
}

// ���ö���Ƕ�
void set_servo_angle(int channel, int angle)
{
    if (angle < 0)
        angle = 0;
    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    int pulsewidth = SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_ANGLE);
    int duty = (pulsewidth * (1 << 13)) / 20000; // ת��Ϊ13λռ�ձ�
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}



#if smooth_flag
// ���ƶ��ƽ���˶���Ŀ��Ƕ�
esp_err_t servo_smooth_move(int pwm_channel,double start_angle,double target_angle, double duration) {
    double step = 0.05;  // ÿ����ֵ���ʱ����
    double current_time = 0;

    // ͨ��ĳ�ַ�����ȡ��ǰ�Ƕȣ���ʼ�� start_angle
    // ���� start_angle �Ѿ����ú�

    while (current_time <= duration) {
        // ��ֵ����Ŀ��Ƕ�
        double angle = start_angle + ((target_angle - start_angle) * (current_time / duration));

        // ESP_LOGI(TAG, "Time: %.2f, Angle: %.2f", current_time, angle);
        // ���ö���Ƕ�
        set_servo_angle(pwm_channel, angle);

        // ��ʱ 100ms��ȷ��ƽ���˶�
        vTaskDelay(pdMS_TO_TICKS(100));
        current_time += step;
    }

    // ȷ�����Ƕ�׼ȷ����
    // servo_set_angle(pwm_channel, target_angle);

    return ESP_OK;
}
#else
double get_servo_angle(int pwm_channel) {
    // ����Ҫ����ʵ��Ӳ���Ϳ��Ʒ�����ʵ���������
    // �����Ϊʾ��
    // ͨ������ĽǶȿ���ͨ����¼�� PWM ռ�ձ�������
    // ʾ�����루α���룩��
    int duty = ledc_get_duty(LEDC_LOW_SPEED_MODE, pwm_channel);
    double pulsewidth = ((double)duty / (1 << 13)) * 20000; // ת��Ϊ����΢�룩
    double angle = ((pulsewidth - SERVO_MIN_PULSEWIDTH) * SERVO_MAX_ANGLE) / (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH);
    return angle;
}


esp_err_t servo_smooth_move(int pwm_channel, double target_angle, double duration) {
    // ��ȡ��ǰ�Ƕ�
    double start_angle = 0;
    // �����ȡ��ǰ�Ƕȵĺ������� get_servo_angle
    // �����ʵ������޸�
    start_angle = get_servo_angle(pwm_channel); 

    double step = 0.05;  // ÿ����ֵ���ʱ����
    double current_time = 0;

    while (current_time <= duration) {
        // ��ֵ����Ŀ��Ƕ�
        double angle = start_angle + ((target_angle - start_angle) * (current_time / duration));

        // ���ö���Ƕ�
        set_servo_angle(pwm_channel, angle);

        // ��ʱ��ȷ��ƽ���˶�
        vTaskDelay(pdMS_TO_TICKS(100));
        current_time += step;
    }

    // ȷ�����Ƕ�׼ȷ����
    set_servo_angle(pwm_channel, target_angle);

    return ESP_OK;
}

#endif