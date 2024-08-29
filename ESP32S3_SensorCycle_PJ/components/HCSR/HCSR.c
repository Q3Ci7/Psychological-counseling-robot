#include "HCSR.h"
#include "esp_timer.h"

// 用于存储回波信号的时间戳
static int64_t start_time, end_time;

void IRAM_ATTR echo_isr_handler(void* arg);

// 初始化 GPIO 引脚
void ultrasonic_init(void) {
    gpio_config_t io_conf;

    // 配置触发引脚
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TRIGGER_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // 配置回波引脚
    io_conf.intr_type = GPIO_INTR_ANYEDGE; // 使用任意边缘中断
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // 安装中断服务
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ECHO_PIN, echo_isr_handler, (void*) ECHO_PIN);
}

// 发送触发脉冲
static void send_trigger_pulse(void) {
    gpio_set_level(TRIGGER_PIN, 0);
    esp_rom_delay_us(2);  // 延时 2 微秒
    gpio_set_level(TRIGGER_PIN, 1);
    esp_rom_delay_us(10); // 延时 10 微秒
    gpio_set_level(TRIGGER_PIN, 0);
}

// 中断处理程序
void IRAM_ATTR echo_isr_handler(void* arg) {
    if (gpio_get_level(ECHO_PIN) == 1) {
        start_time = esp_timer_get_time();
    } else {
        end_time = esp_timer_get_time();
    }
}

// 计算并返回距离
float ultrasonic_get_distance(void) {
    send_trigger_pulse();
    vTaskDelay(100 / portTICK_PERIOD_MS); // 等待回波信号
    
    int64_t duration = end_time - start_time;
    float distance = (duration / 2.0) * 0.0343; // 0.0343 cm/us 为声速
    return distance;
}
