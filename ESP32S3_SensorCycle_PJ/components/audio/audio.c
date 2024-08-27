#include "audio.h"


static const char *UART_TAG = "UART";




/** �������ݰ�У���
 * @param command ָ������������ݰ��������ָ��
 * @param size ����Ĵ�С�����ֽ�Ϊ��λ�����������ݺ�У����ֶ�
*/
uint8_t calculate_checksum(const uint8_t *command, size_t size)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < size - 1; i++) // ע������ʹ�� size - 1
    {
        checksum += command[i];
    }

    return checksum;
}



/** �����������ݰ�
 * @desc �ú���ͨ�� UART ���͸������������ݰ�
 * @param data ָ�����Ҫ���͵����ݰ��������ָ��
 * @param len ���ݰ��ĳ��ȣ����ֽ�Ϊ��λ��
*/
void send_command(const uint8_t *data, size_t len)
{
    uart_write_bytes(UART_NUM_0, (const char *)data, len);
}



/** ��ʼ������Ƶ
*/
void A_play()
{
    uint8_t command[] = {0xAA, 0x02, 0x00, 0x00}; // ��ʼ������У���λ�õ�����

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // ��ӡ���͵������У���
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}



/** ֹͣ������Ƶ
*/
void A_stop()
{
    uint8_t command[] = {0xAA, 0x04, 0x00, 0x00};

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // ��ӡ���͵������У���
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}



/** ѡ����Ƶ��Ŀ
 * @param num ������Ŀѡ��
*/
void A_choose(uint16_t track_num)
{
    uint8_t command[] = {0xAA, 0x07, 0x02, (uint8_t)(track_num >> 8), (uint8_t)(track_num & 0xFF), 0x00};

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // ��ӡ���͵������У���
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}



/** ������Ƶ������Ƶ
 * @param volume ����ѡ��0-30��
*/
void A_setvolume(uint8_t volume)
{
    uint8_t command[] = {0xAA, 0x13, 0x01, volume, 0x00};
    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // ��ӡ���͵������У���
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}

void A_setCyclemode(uint8_t mode)
{
    uint8_t command[] = {0xAA, 0x18, 0x01, mode, 0x00};
    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // ��ӡ���͵������У���
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}



/** ��Ƶ����
 * @param num ������Ŀѡ��
 * @param volume ����ѡ��0-30��
 * @param duration ����ʱ�䣨min��
*/
void Audio(uint8_t num)
{
    // A_setvolume(volume); 
    A_choose(num);
    // vTaskDelay(duration * 60 * 1000 / portTICK_PERIOD_MS);
    // A_stop();
    // vTaskDelay(pdMS_TO_TICKS(100));
}


void Audio_init(uint8_t playmode,uint8_t volume){
    A_setCyclemode(playmode);
    A_setvolume(volume);
    // A_stop();
}