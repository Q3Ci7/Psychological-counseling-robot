#include "gsr.h"


// static const char *TAG = "GSR";
uint8_t emostatu;

// 全局变量
int threshold = 0;

// 初始化 ADC 和 GPIO
void init_hardware() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(GSR_PIN, ADC_ATTEN_DB_11);
}

// 读取 GSR 传感器值
int read_gsr_sensor() {
    // printf("%d\n",sensorValue);
    return adc1_get_raw(GSR_PIN);
}

// 任务：检测情绪变化
void detect_emotion_task(void *pvParameters) {
    int sensorValue;
    while (1) {
        sensorValue = read_gsr_sensor();
        printf("%d\n",sensorValue);
        vTaskDelay(pdMS_TO_TICKS(50)); // 小延迟以防止任务占用过多 CPU
    }
}