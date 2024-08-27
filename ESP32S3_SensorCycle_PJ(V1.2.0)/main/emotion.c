#include "emotion.h"





// 将情绪字符串映射到情绪概率
EmotionProb string_to_emotion_prob(const char* emotion_str) {
    EmotionProb prob = {0};
    if (strcmp(emotion_str, "7") == 0) {
        prob.Angry = 1.0;
    } else if (strcmp(emotion_str, "5") == 0) {
        prob.Disgust = 1.0;
    } else if (strcmp(emotion_str, "6") == 0) {
        prob.Fear = 1.0;
    } else if (strcmp(emotion_str, "2") == 0) {
        prob.Happy = 1.0;
    } else if (strcmp(emotion_str, "4") == 0) {
        prob.Sad = 1.0;
    } else if (strcmp(emotion_str, "1") == 0) {
        prob.Surprise = 1.0;
    } else if (strcmp(emotion_str, "3") == 0) {
        prob.Neutral = 1.0;
    }
    return prob;
}


//定义一个函数，将心率值映射到情绪概率
EmotionProb heart_rate_to_emotion_prob(int HR) {
    EmotionProb prob;
    if (HR < 20) {
        prob = (EmotionProb){0.1, 0.3, 0.1, 0.1, 0.3, 0.05, 0.05};//倾向于中性
    } else if (HR <= 100) {
        prob = (EmotionProb){0.1, 0.1, 0.1, 0.4, 0.1, 0.1, 0.1};//倾向于紧张或兴奋情绪
    } else {
        prob = (EmotionProb){0.3, 0.1, 0.3, 0.1, 0.05, 0.1, 0.05};//倾向于消极或低落情绪
    }
    return prob;
}




//定义一个函数，将面部情绪识别和心率数据对应的情绪概率进行加权平均
EmotionProb combine_emotion(EmotionProb face_prob, EmotionProb hr_prob, float w1, float w2) {
    EmotionProb combined_prob;
    combined_prob.Angry = w1 * face_prob.Angry + w2 * hr_prob.Angry;
    combined_prob.Disgust = w1 * face_prob.Disgust + w2 * hr_prob.Disgust;
    combined_prob.Fear = w1 * face_prob.Fear + w2 * hr_prob.Fear;
    combined_prob.Happy = w1 * face_prob.Happy + w2 * hr_prob.Happy;
    combined_prob.Sad = w1 * face_prob.Sad + w2 * hr_prob.Sad;
    combined_prob.Surprise = w1 * face_prob.Surprise + w2 * hr_prob.Surprise;
    combined_prob.Neutral = w1 * face_prob.Neutral + w2 * hr_prob.Neutral;
    return combined_prob;
}




//选择概率最高的情绪作为最终的判定结果
const char* get_final_emotion(EmotionProb prob) {
    float max_prob = prob.Angry;
    const char* emotion = "Angry";
    
    if (prob.Disgust > max_prob) {
        max_prob = prob.Disgust;
        emotion = "Disgust";
    }
    if (prob.Fear > max_prob) {
        max_prob = prob.Fear;
        emotion = "Fear";
    }
    if (prob.Happy > max_prob) {
        max_prob = prob.Happy;
        emotion = "Happy";
    }
    if (prob.Sad > max_prob) {
        max_prob = prob.Sad;
        emotion = "Sad";
    }
    if (prob.Surprise > max_prob) {
        max_prob = prob.Surprise;
        emotion = "Surprise";
    }
    if (prob.Neutral > max_prob) {
        max_prob = prob.Neutral;
        emotion = "Neutral";
    }
    
    return emotion;
}
