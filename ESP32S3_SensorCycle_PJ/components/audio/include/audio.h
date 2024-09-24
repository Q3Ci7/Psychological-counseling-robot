#ifndef AUDIO_H
#define AUDIO_H

#include "common.h"







// static TaskHandle_t check_playback_task_handle = NULL;
// static SemaphoreHandle_t control_semaphore;
extern volatile bool task_control_flag;//任务控制变量
extern volatile uint8_t end_flag;//任务控制变量






/** 计算数据包校验和
 * @param command 指向包含命令数据包的数组的指针
 * @param size 数组的大小（以字节为单位），包括数据和校验和字段
 */
uint8_t calculate_checksum(const uint8_t *command, size_t size);

/** 发送命令数据包
 * @desc 该函数通过 UART 发送给定的命令数据包
 * @param data 指向包含要发送的数据包的数组的指针
 * @param len 数据包的长度（以字节为单位）
 */
void send_command(const uint8_t *data, size_t len);

/** 开始播放音频
 */
void A_play();

/** 停止播放音频
 */
void A_stop();

/** 选择音频曲目
 * @param num 播放曲目选择
 */
void A_choose(uint16_t track_num);

/** 设置音频音量音频
 * @param volume 音量选择（0-30）
 */
void A_setvolume(uint8_t volume);

/** 串口接收控制音频播放
 * @param pvParameters
 */
void rx_uart_task(void *pvParameters);

/** 音频播放
 * @param num 播放曲目选择
 * @param volume 音量选择（0-30）
 * @param duration 持续时间（min）
 */
void Audio(uint8_t num);
void Audio_init(uint8_t playmode,uint8_t volume);



// void checkplystation();
void checkplystation(void *pvParameter);
#endif