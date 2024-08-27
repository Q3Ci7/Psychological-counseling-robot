#include "emotion.h"


/**
 * @param data 
 * @param Hprob Declare the heart rate probability struct variable.
 * @param first_Receive_Data first data reception flag bit
 * @param start_bpm Record initial heart rate
 * @param growth_rate Record each growth rate
 * @param return
 * @description Process the heart rate data and calculate the emotional probability as a function
 */
void processHeartRateData(bpm_spo2_t data, EmotionProb *Hprob, bool *first_Receive_Data, float *start_bpm, float *growth_rate)
{
    if (!*first_Receive_Data)
    {
        *start_bpm = data.heart_bpm;
        ESP_LOGI("sensorCycle_task:", "The first received heart rate data: %.2f has been stored", *start_bpm);
        *first_Receive_Data = true;
    }
    ESP_LOGI("AVG Heart BPM:", "%.2f", data.heart_bpm);
    *growth_rate = ((float)(data.heart_bpm - *start_bpm) / *start_bpm);
    ESP_LOGI("growth_rate:", "%f", *growth_rate);

    if (*growth_rate < 0)
    {
        *Hprob = (EmotionProb){0.1, 0.3, 0.1, 0.1, 0.3, 0.05, 0.05};
    }
    else if (*growth_rate <= 5)
    {
        *Hprob = (EmotionProb){0.1, 0.1, 0.1, 0.4, 0.1, 0.1, 0.4};
    }
    else
    {
        *Hprob = (EmotionProb){0.3, 0.1, 0.3, 0.1, 0.05, 0.3, 0.05};
    }
}



/**
 * @param msg 
 * @param Fprob Declare the face probability struct variable.
 * @param Hprob Declare the heart rate probability struct variable.
 * @param Zprob Declare the synthetic probability struct variable.
 * @param return
 * @description Process the heart rate data and calculate the emotional probability as a function
 */
void processFacialEmotionData(Message msg, EmotionProb *Fprob, EmotionProb *Hprob, EmotionProb *Zprob)
{
    memset(Fprob, 0, sizeof(EmotionProb)); 
    ESP_LOGI("Received EMO:", "%d", msg.emotype);
    
    switch (msg.emotype)
    {
    case 1:
        Fprob->Surprise = 1.0;
        break;
    case 2:
        Fprob->Happy = 1.0;
        break;
    case 3:
        Fprob->Neutral = 1.0;
        break;
    case 4:
        Fprob->Sad = 1.0;
        break;
    case 5:
        Fprob->Disgust = 1.0;
        break;
    case 6:
        Fprob->Fear = 1.0;
        break;
    case 7:
        Fprob->Angry = 1.0;
        break;
    case 8:
        Audio_init(2, 25);
        A_choose(4);
        char *send_data = "plydone";
        uart_write_bytes(UART_NUM_2, send_data, strlen(send_data));
        break;
    default:
        break;
    }
}



/**
 * @param Fprob Declare the face probability struct variable.
 * @param Hprob Declare the heart rate probability struct variable.
 * @param Zprob Declare the synthetic probability struct variable.
 * @param return
 * @description A function that synthesizes the final emotion probability
 */
void calculateFinalEmotionProbabilities(EmotionProb *Fprob, EmotionProb *Hprob, EmotionProb *Zprob)
{
    Zprob->Angry = W1 * Fprob->Angry + W2 * Hprob->Angry;
    Zprob->Disgust = W1 * Fprob->Disgust + W2 * Hprob->Disgust;
    Zprob->Fear = W1 * Fprob->Fear + W2 * Hprob->Fear;
    Zprob->Happy = W1 * Fprob->Happy + W2 * Hprob->Happy;
    Zprob->Sad = W1 * Fprob->Sad + W2 * Hprob->Sad;
    Zprob->Surprise = W1 * Fprob->Surprise + W2 * Hprob->Surprise;
    Zprob->Neutral = W1 * Fprob->Neutral + W2 * Hprob->Neutral;
}



/**
 * @param Zprob Declare the synthetic probability struct variable.
 * @param music_1_playing normal emotion music flag
 * @param music_2_playing positive  emotion music flag
 * @param music_3_playing negative emotion music flag
 * @param return
 * @description A function that performs an action based on an emotional outcome
 */
void performActionsBasedOnEmotion(EmotionProb *Zprob, bool *music_1_playing, bool *music_2_playing, bool *music_3_playing)
{
    float values[] = {Zprob->Angry, Zprob->Disgust, Zprob->Fear, Zprob->Happy, Zprob->Sad, Zprob->Surprise, Zprob->Neutral};
    uint8_t max_index = 7;
    
    for (int i = 0; i < 7; ++i)
    {
        if (values[i] > values[max_index])
        {
            max_index = i;
        }
    }

    switch (max_index)
    {
    case 0:
        ESP_LOGI("final emo", "angry");
        ws2812_write(ws2812_handle, idx, 250, 0, 0);
        if (!*music_3_playing)
        {
            Audio_init(1, 30);
            A_choose(3);
            *music_1_playing = false;
            *music_2_playing = false;
            *music_3_playing = true;
        }
        warm_stop();
        uart_write_bytes(UART_NUM_2, "angry", strlen("angry"));
        break;
    case 1:
        ESP_LOGI("final emo", "disgust");
        ws2812_write(ws2812_handle, idx, 0, 250, 0);
        if (!*music_3_playing)
        {
            Audio_init(1, 30);
            A_choose(3);
            *music_1_playing = false;
            *music_2_playing = false;
            *music_3_playing = true;
        }
        warm_stop();
        uart_write_bytes(UART_NUM_2, "disgust", strlen("disgust"));
        break;
    case 2:
        ESP_LOGI("final emo", "fear");
        ws2812_write(ws2812_handle, idx, 0, 0, 128);
        if (!*music_3_playing)
        {
            Audio_init(1, 30);
            A_choose(3);
            *music_1_playing = false;
            *music_2_playing = false;
            *music_3_playing = true;
        }
        warm_begin();
        uart_write_bytes(UART_NUM_2, "fear", strlen("fear"));
        break;
    case 3:
        ESP_LOGI("final emo", "happy");
        ws2812_write(ws2812_handle, idx, 255, 255, 0);
        if (!*music_2_playing)
        {
            Audio_init(1, 25);
            A_choose(2);
            *music_1_playing = false;
            *music_2_playing = true;
            *music_3_playing = false;
        }
        warm_stop();
        uart_write_bytes(UART_NUM_2, "happy", strlen("happy"));
        break;
    case 4:
        ESP_LOGI("final emo", "Sad");
        ws2812_write(ws2812_handle, idx, 135, 206, 235);
        if (!*music_3_playing)
        {
            Audio_init(1, 30);
            A_choose(3);
            *music_1_playing = false;
            *music_2_playing = false;
            *music_3_playing = true;
        }
        warm_begin();
        uart_write_bytes(UART_NUM_2, "sad", strlen("sad"));
        break;
    case 5:
        ESP_LOGI("final emo", "surprise");
        ws2812_write(ws2812_handle, idx, 128, 0, 128);
        if (!*music_2_playing)
        {
            Audio_init(1, 25);
            A_choose(2);
            *music_1_playing = false;
            *music_2_playing = true;
            *music_3_playing = false;
        }
        warm_stop();
        uart_write_bytes(UART_NUM_2, "surprise", strlen("surprise"));
        break;
    case 6:
        ESP_LOGI("final emo", "Neutral");
        ws2812_write(ws2812_handle, idx, 192, 192, 192);
        if (!*music_1_playing)
        {
            Audio_init(1, 25);
            A_choose(1);
            *music_1_playing = true;
            *music_2_playing = false;
            *music_3_playing = false;
        }
        warm_stop();
        uart_write_bytes(UART_NUM_2, "neutral", strlen("neutral"));
        break;
    default:
        break;
    }
}






