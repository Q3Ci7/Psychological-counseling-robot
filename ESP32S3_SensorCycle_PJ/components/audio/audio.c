#include "audio.h"

static const char *UART_TAG = "UART";
volatile bool sdbk_flag = false;
#define BUF_0_SIZE 1024

/** 计算数据包校验和
 * @param command 指向包含命令数据包的数组的指针
 * @param size 数组的大小（以字节为单位），包括数据和校验和字段
 */
uint8_t calculate_checksum(const uint8_t *command, size_t size)
{
    uint8_t checksum = 0;
    for (size_t i = 0; i < size - 1; i++) // 注意这里使用 size - 1
    {
        checksum += command[i];
    }

    return checksum;
}

/** 发送命令数据包
 * @desc 该函数通过 UART 发送给定的命令数据包
 * @param data 指向包含要发送的数据包的数组的指针
 * @param len 数据包的长度（以字节为单位）
 */
void send_command(const uint8_t *data, size_t len)
{
    uart_write_bytes(UART_NUM_1, (const char *)data, len);
}

/** 开始播放音频
 */
void A_play()
{
    uint8_t command[] = {0xAA, 0x02, 0x00, 0x00}; // 初始化包含校验和位置的数组

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // 打印发送的命令和校验和
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}

/** 停止播放音频
 */
void A_stop()
{
    uint8_t command[] = {0xAA, 0x04, 0x00, 0x00};

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // 打印发送的命令和校验和
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}

/** 选择音频曲目
 * @param num 播放曲目选择
 */
void A_choose(uint16_t track_num)
{

    uint8_t command[] = {0xAA, 0x07, 0x02, (uint8_t)(track_num >> 8), (uint8_t)(track_num & 0xFF), 0x00};

    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // 打印发送的命令和校验和
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}

/** 设置音频音量音频
 * @param volume 音量选择（0-30）
 */
void A_setvolume(uint8_t volume)
{
    uint8_t command[] = {0xAA, 0x13, 0x01, volume, 0x00};
    uint8_t checksum = calculate_checksum(command, sizeof(command));
    command[sizeof(command) - 1] = checksum;
    send_command(command, sizeof(command));

    // 打印发送的命令和校验和
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

    // 打印发送的命令和校验和
    ESP_LOGI(UART_TAG, "Command sent:");
    for (int i = 0; i < sizeof(command); i++)
    {
        ESP_LOGI(UART_TAG, "command[%d]: %02X ", i, command[i]);
    }
}

void checkplystation(void *pvParameter)
{
    uint8_t data[BUF_0_SIZE];
    uint8_t command[] = {0xAA, 0x01, 0x00, 0xAB};
    const uint8_t sequence1[] = {0xaa, 0x01, 0x01, 0x00, 0xac}; // 停止

    size_t length = sizeof(sequence1) / sizeof(sequence1[0]);

    while (1)
    {

        if (task_control_flag)
        {
            send_command(command, sizeof(command));
            vTaskDelay(pdMS_TO_TICKS(500));
            int len = uart_read_bytes(UART_NUM_1, data, BUF_0_SIZE, 100 / portTICK_PERIOD_MS);

            if (len > 0)
            {
                int result = memcmp(data, sequence1, length);
                if (result == 0)
                {
                    task_control_flag = false;

                    
                    if (end_flag == 0)
                    {
                        if (sdbk_flag == true)
                        {
                            uart_write_bytes(UART_NUM_2, "selfend", strlen("selfend"));
                        }
                    }

                    else if (end_flag == 1)
                    {
                        uart_write_bytes(UART_NUM_2, "picend", strlen("picend"));
                    }
                    else
                    {
                        uart_write_bytes(UART_NUM_2, "playend", strlen("playend"));
                    }

                    ESP_LOGI("ply_statu", "0");
                }
                else
                {
                    ESP_LOGI("ply_statu", "1");
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/** 音频播放
 * @param num 播放曲目选择
 * @param volume 音量选择（0-30）
 * @param duration 持续时间（min）
 */
void Audio(uint8_t num)
{
    // A_setvolume(volume);
    A_choose(num);
    // vTaskDelay(duration * 60 * 1000 / portTICK_PERIOD_MS);
    // A_stop();
    // vTaskDelay(pdMS_TO_TICKS(100));
}

void Audio_init(uint8_t playmode, uint8_t volume)
{
    A_setCyclemode(playmode);
    A_setvolume(volume);
    // A_stop();
}