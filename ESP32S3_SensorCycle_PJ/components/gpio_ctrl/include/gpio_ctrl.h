#ifndef GPIO_CTRL_H
#define GPIO_CTRL_H

#include "common.h"


#define JIARE_GPIO_PIN 2 
#define WUHUA_GPIO_PIN1 3 
#define WUHUA_GPIO_PIN2 4

void mygpio_init();

void warm_begin();
void warm_stop();

void Wuhua_1_begin();
void Wuhua_1_stop();

void Wuhua_2_begin();
void Wuhua_2_stop();

#endif