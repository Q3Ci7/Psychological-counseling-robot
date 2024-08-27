#ifndef EMOTION_H
#define EMOTION_H

#include "common.h"

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


EmotionProb string_to_emotion_prob(const char* emotion_str);
EmotionProb heart_rate_to_emotion_prob(int HR);
EmotionProb combine_emotion(EmotionProb face_prob, EmotionProb hr_prob, float w1, float w2);
const char* get_final_emotion(EmotionProb prob);

#endif
