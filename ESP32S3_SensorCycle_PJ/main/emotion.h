#ifndef EMOTION_H
#define EMOTION_H

#include "common.h"
#include "bpm.h"
#include "ws2812b.h"
#include "gpio_ctrl.h"
#include "audio.h"

//定义权重
#define W1 0.7
#define W2 0.3
//定义权情绪概率结构
typedef struct {
    float Angry;
    float Disgust;
    float Fear;
    float Happy;
    float Sad;
    float Surprise;
    float Neutral;
} EmotionProb;


// extern ws2812_strip_handle_t ws2812_handle;
// extern int idx;


// void processHeartRateData(bpm_spo2_t data, EmotionProb *Hprob, bool *first_Receive_Data, float *start_bpm, float *growth_rate);
// void processFacialEmotionData(Message msg, EmotionProb *Fprob, EmotionProb *Hprob, EmotionProb *Zprob);
// void calculateFinalEmotionProbabilities(EmotionProb *Fprob, EmotionProb *Hprob, EmotionProb *Zprob);
// void performActionsBasedOnEmotion(EmotionProb *Zprob, bool *music_1_playing, bool *music_2_playing, bool *music_3_playing);


#endif
