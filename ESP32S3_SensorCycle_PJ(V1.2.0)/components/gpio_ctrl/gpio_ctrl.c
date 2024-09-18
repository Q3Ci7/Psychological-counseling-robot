#include "gpio_ctrl.h"

void mygpio_init()
{
    gpio_reset_pin(JIARE_GPIO_PIN);
    gpio_reset_pin(WUHUA_GPIO_PIN1);
    gpio_reset_pin(WUHUA_GPIO_PIN2);

    gpio_set_direction(JIARE_GPIO_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(WUHUA_GPIO_PIN1, GPIO_MODE_OUTPUT);
    gpio_set_direction(WUHUA_GPIO_PIN2, GPIO_MODE_OUTPUT);
}

void warm_begin()
{
    gpio_set_level(JIARE_GPIO_PIN, 1);
}
void warm_stop()
{
    gpio_set_level(JIARE_GPIO_PIN, 0);
}

void Wuhua_1_begin()
{
    gpio_set_level(WUHUA_GPIO_PIN1, 0);
}
void Wuhua_1_stop()
{
    gpio_set_level(WUHUA_GPIO_PIN1, 1);
}


void Wuhua_2_begin()
{
    gpio_set_level(WUHUA_GPIO_PIN2, 0);
}
void Wuhua_2_stop()
{
    gpio_set_level(WUHUA_GPIO_PIN2, 1);
}