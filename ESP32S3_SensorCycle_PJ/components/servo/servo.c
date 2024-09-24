#include "servo.h"

/*
*
*
*
*
* * @brief 舵机控制程序
 * 
 * 该程序使用 ESP32 的 LEDC（LED控制器）模块来控制舵机的 PWM（脉宽调制）信号。LEDC 模块能够生成高精度的 PWM 信号，适用于舵机控制。
 * 
 * LEDC 模块配置的主要步骤如下：
 * 
 * 1. **初始化舵机**：
 *    - 配置 LEDC 定时器，设置 PWM 的频率和分辨率。常用于控制舵机的 PWM 信号频率为50Hz（标准舵机频率）。
 *    - 配置 LEDC 通道，设置 PWM 信号的输出引脚和初始占空比。每个通道可以控制一个舵机。
 * 
 * 2. **设置舵机角度**：
 *    - 通过将目标角度转换为对应的 PWM 脉宽，调整 PWM 的占空比，从而控制舵机的位置。
 *    - 计算和设置 PWM 占空比，使其在舵机可接受的脉宽范围内（通常在500μs到2500μs之间）。
 * 
 * 函数说明：
 * 
 * - `servo_init(int channel, int gpio_num)`：
 *   初始化指定通道的 PWM 控制，以控制舵机的运动。配置 LEDC 定时器（包括频率和分辨率），并设置 LEDC 通道的 GPIO 引脚和初始占空比。
 * 
 * - `set_servo_angle(int channel, int angle)`：
 *   根据给定的角度计算 PWM 脉宽，并设置 PWM 占空比以调整舵机的位置。角度值被限制在有效范围内，脉宽被转换为 13 位的占空比。
 *
****************************************************************************************************/
// 初始化单个舵机
void servo_init(int channel, int gpio_num)
{
    // 配置 LEDC 定时器，用于生成伺服电机所需的PWM信号
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // 设置占空比分辨率为13位，表示2^13=8192个级别
        .freq_hz = 50,                        // PWM频率设置为50Hz（适合伺服电机的标准控制频率）
        .speed_mode = LEDC_LOW_SPEED_MODE,    // 使用低速模式（LEDC低速通道）
        .timer_num = LEDC_TIMER_0             // 使用定时器0
    };
    ledc_timer_config(&ledc_timer); // 应用定时器配置

    // 配置 LEDC 通道，用于输出PWM信号到指定GPIO引脚
    ledc_channel_config_t ledc_channel = {
        .channel = (ledc_channel_t)channel,   // LEDC通道号，通过传入参数指定（0~7通道）
        .duty = 0,                            // 初始占空比设为0，表示伺服电机不移动
        .gpio_num = gpio_num,                 // 输出PWM信号的GPIO引脚，通过传入参数指定
        .speed_mode = LEDC_LOW_SPEED_MODE,    // 使用低速模式（与定时器的模式匹配）
        .hpoint = 0,                          // 设置脉冲在计时器周期中的起始位置（默认设为0）
        .timer_sel = LEDC_TIMER_0             // 选择LEDC定时器0（与上面的定时器配置一致）
    };
    ledc_channel_config(&ledc_channel);       // 应用通道配置
}

void servo_deinit(int channel)
{
    // 停止通道输出
    ledc_stop(LEDC_LOW_SPEED_MODE, channel, 0);

    // 清除通道配置
    ledc_channel_config_t ledc_channel = {
        .channel = (ledc_channel_t)channel,   // LEDC通道号，通过传入参数指定（0~7通道）
        .duty = 0,                            // 占空比设为0，确保停止输出
        .gpio_num = -1,                       // 无效GPIO引脚
        .speed_mode = LEDC_LOW_SPEED_MODE,    // 使用低速模式
        .hpoint = 0,                          // 设置脉冲在计时器周期中的起始位置（默认设为0）
        .timer_sel = LEDC_TIMER_0             // 选择LEDC定时器0（与上面的定时器配置一致）
    };
    ledc_channel_config(&ledc_channel);       // 应用清除通道配置

    // 如果所有舵机通道都不再使用，可以停止并清除定时器配置
    // 这里只是停止定时器的示例，实际应用中可能需要根据情况进行修改
    ledc_timer_pause(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);  // 暂停定时器
    ledc_timer_rst(LEDC_LOW_SPEED_MODE, LEDC_TIMER_0);    // 重置定时器
}

// 设置舵机角度
void set_servo_angle(int channel, int angle)
{
    if (angle < 0)
        angle = 0;
    if (angle > SERVO_MAX_ANGLE)
        angle = SERVO_MAX_ANGLE;

    int pulsewidth = SERVO_MIN_PULSEWIDTH + (angle * (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) / SERVO_MAX_ANGLE);
    int duty = (pulsewidth * (1 << 13)) / 20000; // 转换为13位占空比
    ledc_set_duty(LEDC_LOW_SPEED_MODE, channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, channel);
}



#if smooth_flag
// 控制舵机平滑运动到目标角度
esp_err_t servo_smooth_move(int pwm_channel,double start_angle,double target_angle, double duration) {
    double step = 0.05;  // 每个插值点的时间间隔
    double current_time = 0;

    // 通过某种方法获取当前角度，初始化 start_angle
    // 假设 start_angle 已经设置好

    while (current_time <= duration) {
        // 插值计算目标角度
        double angle = start_angle + ((target_angle - start_angle) * (current_time / duration));

        // ESP_LOGI(TAG, "Time: %.2f, Angle: %.2f", current_time, angle);
        // 设置舵机角度
        set_servo_angle(pwm_channel, angle);

        // 延时 100ms，确保平滑运动
        vTaskDelay(pdMS_TO_TICKS(100));
        current_time += step;
    }

    // 确保最后角度准确设置
    // servo_set_angle(pwm_channel, target_angle);

    return ESP_OK;
}
#else
double get_servo_angle(int pwm_channel) {
    // 你需要根据实际硬件和控制方法来实现这个函数
    // 这里仅为示例
    // 通常舵机的角度可以通过记录的 PWM 占空比来计算
    // 示例代码（伪代码）：
    int duty = ledc_get_duty(LEDC_LOW_SPEED_MODE, pwm_channel);
    double pulsewidth = ((double)duty / (1 << 13)) * 20000; // 转换为脉宽（微秒）
    double angle = ((pulsewidth - SERVO_MIN_PULSEWIDTH) * SERVO_MAX_ANGLE) / (SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH);
    return angle;
}


esp_err_t servo_smooth_move(int pwm_channel, double target_angle, double duration) {
    // 获取当前角度
    double start_angle = 0;
    // 假设获取当前角度的函数叫做 get_servo_angle
    // 请根据实际情况修改
    start_angle = get_servo_angle(pwm_channel); 

    double step = 0.05;  // 每个插值点的时间间隔
    double current_time = 0;

    while (current_time <= duration) {
        // 插值计算目标角度
        double angle = start_angle + ((target_angle - start_angle) * (current_time / duration));

        // 设置舵机角度
        set_servo_angle(pwm_channel, angle);

        // 延时，确保平滑运动
        vTaskDelay(pdMS_TO_TICKS(100));
        current_time += step;
    }

    // 确保最后角度准确设置
    set_servo_angle(pwm_channel, target_angle);

    return ESP_OK;
}

#endif