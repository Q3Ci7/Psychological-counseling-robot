#ifndef AUDIO_H
#define AUDIO_H

#include "common.h"







// static TaskHandle_t check_playback_task_handle = NULL;
// static SemaphoreHandle_t control_semaphore;
extern volatile bool task_control_flag;//������Ʊ���







/** �������ݰ�У���
 * @param command ָ������������ݰ��������ָ��
 * @param size ����Ĵ�С�����ֽ�Ϊ��λ�����������ݺ�У����ֶ�
 */
uint8_t calculate_checksum(const uint8_t *command, size_t size);

/** �����������ݰ�
 * @desc �ú���ͨ�� UART ���͸������������ݰ�
 * @param data ָ�����Ҫ���͵����ݰ��������ָ��
 * @param len ���ݰ��ĳ��ȣ����ֽ�Ϊ��λ��
 */
void send_command(const uint8_t *data, size_t len);

/** ��ʼ������Ƶ
 */
void A_play();

/** ֹͣ������Ƶ
 */
void A_stop();

/** ѡ����Ƶ��Ŀ
 * @param num ������Ŀѡ��
 */
void A_choose(uint16_t track_num);

/** ������Ƶ������Ƶ
 * @param volume ����ѡ��0-30��
 */
void A_setvolume(uint8_t volume);

/** ���ڽ��տ�����Ƶ����
 * @param pvParameters
 */
void rx_uart_task(void *pvParameters);

/** ��Ƶ����
 * @param num ������Ŀѡ��
 * @param volume ����ѡ��0-30��
 * @param duration ����ʱ�䣨min��
 */
void Audio(uint8_t num);
void Audio_init(uint8_t playmode,uint8_t volume);



// void checkplystation();
void checkplystation(void *pvParameter);
#endif