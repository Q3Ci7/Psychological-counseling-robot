#include "gsr.h"


// static const char *TAG = "GSR";
uint8_t emostatu;

// ȫ�ֱ���
int threshold = 0;

// ��ʼ�� ADC �� GPIO
void init_hardware() {
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(GSR_PIN, ADC_ATTEN_DB_11);
}

// ��ȡ GSR ������ֵ
int read_gsr_sensor() {
    // printf("%d\n",sensorValue);
    return adc1_get_raw(GSR_PIN);
}

// ���񣺼�������仯
void detect_emotion_task(void *pvParameters) {
    int sensorValue;
    while (1) {
        sensorValue = read_gsr_sensor();
        printf("%d\n",sensorValue);
        vTaskDelay(pdMS_TO_TICKS(50)); // С�ӳ��Է�ֹ����ռ�ù��� CPU
    }
}