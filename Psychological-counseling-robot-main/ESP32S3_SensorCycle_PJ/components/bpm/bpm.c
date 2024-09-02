#include "bpm.h"

max30102_config_t max30102 = {};

esp_err_t i2c_master_init(i2c_port_t i2c_port)
{
    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = I2C_SDA;
    conf.scl_io_num = I2C_SCL;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_FRQ;
    i2c_param_config(i2c_port, &conf);
    return i2c_driver_install(i2c_port, I2C_MODE_MASTER, 0, 0, 0);
}
void i2c_init()
{
    // Init I2C_NUM_0
    ESP_ERROR_CHECK(i2c_master_init(I2C_PORT));
    // Init sensor at I2C_NUM_0
    ESP_ERROR_CHECK(max30102_init(&max30102, I2C_PORT,
                                  MAX30102_DEFAULT_OPERATING_MODE,
                                  MAX30102_DEFAULT_SAMPLING_RATE,
                                  MAX30102_DEFAULT_LED_PULSE_WIDTH,
                                  MAX30102_DEFAULT_IR_LED_CURRENT,
                                  MAX30102_DEFAULT_START_RED_LED_CURRENT,
                                  MAX30102_DEFAULT_MEAN_FILTER_SIZE,
                                  MAX30102_DEFAULT_PULSE_BPM_SAMPLE_SIZE,
                                  MAX30102_DEFAULT_ADC_RANGE,
                                  MAX30102_DEFAULT_SAMPLE_AVERAGING,
                                  MAX30102_DEFAULT_ROLL_OVER,
                                  MAX30102_DEFAULT_ALMOST_FULL,
                                  false));
}

void get_bpm_task(void *param)
{

    bpm_spo2_t data; // �����ṹ�����

    float sum_heart_bpm = 0.0;                   // �����ܵ��������ڼ���ƽ��ֵ
    int count = 0;                               // ���嶨������������ڼ���ƽ��ֵ
    TickType_t start_time = xTaskGetTickCount(); // ��¼����ʼʱ��

    max30102_data_t result = {};
    /*ESP_ERROR_CHECK(max30102_print_registers(&max30102));*/
    while (true)
    {
        ESP_ERROR_CHECK(max30102_update(&max30102, &result));

        if (result.pulse_detected) // �ж��Ƿ��⵽���ʣ��ۼ�
        {
            ESP_LOGI("find count + 1 ;current bpm:", "%.2f", result.heart_bpm);
            sum_heart_bpm += result.heart_bpm;
            count++;
        }

        // ����Ƿ񾭹���30s
        TickType_t current_time = xTaskGetTickCount();
        TickType_t elapsed_time = current_time - start_time;

        if (elapsed_time >= 30000 / portTICK_PERIOD_MS) // 30s
        {
            ESP_LOGI("elapsed_time", "30 seconds is up");
            if (count > 0)
            {
                float avg_heart_bpm = sum_heart_bpm / count; // ����ƽ������

                data.heart_bpm = avg_heart_bpm; // ׼��Ҫ���͵�����

                // �����ݷ��͵�����
                if (xQueueSend(data_queue1, &data, portMAX_DELAY) != pdPASS)
                {
                    ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
                }
                ESP_LOGI("avg_heart_bpm", ""
                                          "alread send!!!"
                                          "");
            }
            // �ڽ�������30���������ñ���
            sum_heart_bpm = 0.0;
            count = 0;
            start_time = current_time;
        }

        // Update rate: 100Hz
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
