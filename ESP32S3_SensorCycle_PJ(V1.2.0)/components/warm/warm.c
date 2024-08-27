#include "warm.h"

void mygpio_init()
{
    gpio_reset_pin(GPIO_PIN);
    gpio_set_direction(GPIO_PIN, GPIO_MODE_OUTPUT);
}

void warm_begin()
{
    gpio_set_level(GPIO_PIN, 1);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    // gpio_set_level(GPIO_PIN, 0);
    // vTaskDelay(pdMS_TO_TICKS(1000));
    ESP_LOGI("warm", "warm_begin!!!");
}

void warm_stop()
{
    gpio_set_level(GPIO_PIN, 0);
    ESP_LOGI("warm", "warm_stop!!!");
}