#include "HCSR.h"
#include "esp_timer.h"

// ���ڴ洢�ز��źŵ�ʱ���
static int64_t start_time, end_time;

void IRAM_ATTR echo_isr_handler(void *arg);

// ��ʼ�� GPIO ����
void ultrasonic_init(void)
{
    gpio_config_t io_conf;

    // ���ô�������
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TRIGGER_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);

    // ���ûز�����
    io_conf.intr_type = GPIO_INTR_ANYEDGE; // ʹ�������Ե�ж�
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ECHO_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);

    // ��װ�жϷ���
    gpio_install_isr_service(0);
    gpio_isr_handler_add(ECHO_PIN, echo_isr_handler, (void *)ECHO_PIN);
}

// �ͷų�������������Դ
void ultrasonic_deinit(void)
{
    // �Ƴ� GPIO �жϴ������
    gpio_isr_handler_remove(ECHO_PIN);

    // ж���жϷ���
    gpio_uninstall_isr_service();

    // ���� GPIO ΪĬ��״̬����ѡ��
    gpio_set_direction(TRIGGER_PIN, GPIO_MODE_DISABLE);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_DISABLE);
}

// ���ʹ�������
static void send_trigger_pulse(void)
{
    gpio_set_level(TRIGGER_PIN, 0);
    esp_rom_delay_us(2); // ��ʱ 2 ΢��
    gpio_set_level(TRIGGER_PIN, 1);
    esp_rom_delay_us(10); // ��ʱ 10 ΢��
    gpio_set_level(TRIGGER_PIN, 0);
}

// �жϴ������
void IRAM_ATTR echo_isr_handler(void *arg)
{
    if (gpio_get_level(ECHO_PIN) == 1)
    {
        start_time = esp_timer_get_time();
    }
    else
    {
        end_time = esp_timer_get_time();
    }
}

// ���㲢���ؾ���
float ultrasonic_get_distance(void)
{
    send_trigger_pulse();
    vTaskDelay(100 / portTICK_PERIOD_MS); // �ȴ��ز��ź�

    int64_t duration = end_time - start_time;
    float distance = (duration / 2.0) * 0.0343; // 0.0343 cm/us Ϊ����
    return distance;
}

void Get_Distance_task(void *pvParameters)
{
    float hcsrdata;

    while (1)
    {
        hcsrdata = ultrasonic_get_distance(); // ��ȡ��ǰ�Ƕȵľ���
        if (xQueueSend(distanceQueue, &hcsrdata, portMAX_DELAY) != pdPASS)
        {
            ESP_LOGI("xQueueSend error:", "Failed to send data to queue");
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}