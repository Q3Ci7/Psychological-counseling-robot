#ifndef GSR_H
#define GSR_H

#include "common.h"

#define GSR_PIN ADC1_CHANNEL_7 // GPIO8


void init_hardware();
int read_gsr_sensor();
void detect_emotion_task(void *pvParameters);

#endif