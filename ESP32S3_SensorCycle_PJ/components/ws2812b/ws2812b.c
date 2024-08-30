#include "esp_check.h"
#include "ws2812b.h"
#include "driver/rmt_tx.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
static const char *TAG = "led_encoder";

#define LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz �ֱ���, Ҳ����1tick = 0.1us��Ҳ���ǿ��Կ��Ƶ���Сʱ�䵥Ԫ������0.1us�������޷�����

//WS2812������������
struct ws2812_strip_t
{
    rmt_channel_handle_t led_chan;          //rmtͨ��
    rmt_encoder_handle_t led_encoder;       //rmt������
    uint8_t *led_buffer;                    //rgb����
    int led_num;                            //led����
};

//�Զ��������
typedef struct {
    rmt_encoder_t base;                     //���������������������Ҫ�û�ʵ�ֵĻص�������encode,del,ret
    rmt_encoder_t *bytes_encoder;           //�ֽڱ�����������rmt_new_bytes_encoder�����󴴽�
    rmt_encoder_t *copy_encoder;            //����������������rmt_new_copy_encoder�����󴴽�
    int state;                              //״̬����
    rmt_symbol_word_t reset_code;           //����λ��ʱ��
} rmt_led_strip_encoder_t;

/* ����WS2812���ݵĺ�������˳������
 * 1������rmt_transmit���贫��RMTͨ�������͵����ݡ�����������
 * 2�����ñ�������encode�������ڱ������о��ǵ���rmt_encode_led_strip����
 * 3��������rmt_new_bytes_encoder�������ֽڱ��������뺯��bytes_encoder->encode�����û����ݱ����rmt_symbol_word_t RMT����
 * 4��������rmt_new_copy_encoder�����Ŀ������������뺯��copy_encoder->encode������λ�źŰ�װ�ȶ��ĵ�ƽʱ����б���
 * 5��rmt_encode_led_strip�������أ��ڵײ㽫�źŷ��ͳ�ȥ���������ǲ���IO�ܽŸߵ͵�ƽ��
*/

/** ����ص�����
 * @param encoder ������
 * @param channel RMTͨ��
 * @param primary_data �������û�����
 * @param data_size �������û����ݳ���
 * @param ret_state ����״̬
 * @return RMT���Ÿ���
*/
static size_t rmt_encode_led_strip(rmt_encoder_t *encoder, rmt_channel_handle_t channel, const void *primary_data, size_t data_size, rmt_encode_state_t *ret_state)
{
    /*
    __containerof�������:
    ͨ���ṹ�ĳ�Ա����������ṹ�ĵ�ַ
    ����������У��������encoder��rmt_led_strip_encoder_t�ṹ���е�base��Ա
    __containerof��ͨ��encoder�ĵ�ַ������rmt_led_strip_encoder_t���ڴ��Ų��ҵ�rmt_led_strip_encoder_t* ���׵�ַ
    */
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_handle_t bytes_encoder = led_encoder->bytes_encoder;        //ȡ���ֽڱ�����
    rmt_encoder_handle_t copy_encoder = led_encoder->copy_encoder;          //ȡ������������
    rmt_encode_state_t session_state = RMT_ENCODING_RESET;
    rmt_encode_state_t state = RMT_ENCODING_RESET;
    size_t encoded_symbols = 0;
    switch (led_encoder->state) {   //led_encoder->state���Զ����״̬������ֻ������ֵ��0�Ƿ���RGB���ݣ�1�Ƿ��͸�λ��
    case 0: // send RGB data
        encoded_symbols += bytes_encoder->encode(bytes_encoder, channel, primary_data, data_size, &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {    //�ֽڱ������
            led_encoder->state = 1; // switch to next state when current encoding session finished
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {    //���治�㣬�����˳�
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }
    // fall-through
    case 1: // send reset code
        encoded_symbols += copy_encoder->encode(copy_encoder, channel, &led_encoder->reset_code,
                                                sizeof(led_encoder->reset_code), &session_state);
        if (session_state & RMT_ENCODING_COMPLETE) {
            led_encoder->state = RMT_ENCODING_RESET; // back to the initial encoding session
            state |= RMT_ENCODING_COMPLETE;
        }
        if (session_state & RMT_ENCODING_MEM_FULL) {
            state |= RMT_ENCODING_MEM_FULL;
            goto out; // yield if there's no free space for encoding artifacts
        }
    }
out:
    *ret_state = state;
    return encoded_symbols;
}

static esp_err_t rmt_del_led_strip_encoder(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_del_encoder(led_encoder->bytes_encoder);
    rmt_del_encoder(led_encoder->copy_encoder);
    free(led_encoder);
    return ESP_OK;
}

static esp_err_t rmt_led_strip_encoder_reset(rmt_encoder_t *encoder)
{
    rmt_led_strip_encoder_t *led_encoder = __containerof(encoder, rmt_led_strip_encoder_t, base);
    rmt_encoder_reset(led_encoder->bytes_encoder);
    rmt_encoder_reset(led_encoder->copy_encoder);
    led_encoder->state = RMT_ENCODING_RESET;
    return ESP_OK;
}

/** ����һ������WS2812ʱ��ı�����
 * @param ret_encoder ���صı������������������ʹ��rmt_transmit��������ʱ���õ�
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t rmt_new_led_strip_encoder(rmt_encoder_handle_t *ret_encoder)
{
    esp_err_t ret = ESP_OK;

    //����һ���Զ���ı������ṹ�壬���ڿ��Ʒ��ͱ��������
    rmt_led_strip_encoder_t *led_encoder = NULL;
    led_encoder = calloc(1, sizeof(rmt_led_strip_encoder_t));
    ESP_GOTO_ON_FALSE(led_encoder, ESP_ERR_NO_MEM, err, TAG, "no mem for led strip encoder");
    led_encoder->base.encode = rmt_encode_led_strip;    //�����������rmt�������ݵ�ʱ�򱻵��ã����ǿ���������������Ӷ��������п���
    led_encoder->base.del = rmt_del_led_strip_encoder;  //���������ж��rmtʱ������
    led_encoder->base.reset = rmt_led_strip_encoder_reset;  //��������ڸ�λrmtʱ������

    //�½�һ������������(0,1λ����ʱ�䣬�ο�оƬ�ֲ�)
    rmt_bytes_encoder_config_t bytes_encoder_config = {
        .bit0 = {
            .level0 = 1,
            .duration0 = 0.3 * LED_STRIP_RESOLUTION_HZ / 1000000, // T0H=0.3us
            .level1 = 0,
            .duration1 = 0.9 * LED_STRIP_RESOLUTION_HZ / 1000000, // T0L=0.9us
        },
        .bit1 = {
            .level0 = 1,
            .duration0 = 0.9 * LED_STRIP_RESOLUTION_HZ / 1000000, // T1H=0.9us
            .level1 = 0,
            .duration1 = 0.3 * LED_STRIP_RESOLUTION_HZ / 1000000, // T1L=0.3us
        },
        .flags.msb_first = 1 //��λ�ȴ���
    };
    //������������ã�������ݱ������������
    rmt_new_bytes_encoder(&bytes_encoder_config, &led_encoder->bytes_encoder);

    //�½�һ���������������ã�����������һ�����ڴ���㶨���ַ����ݣ�����˵����λ
    rmt_copy_encoder_config_t copy_encoder_config = {};
    rmt_new_copy_encoder(&copy_encoder_config, &led_encoder->copy_encoder);

    //�趨����λʱ��
    uint32_t reset_ticks = LED_STRIP_RESOLUTION_HZ / 1000000 * 50 / 2; //�ֱ���/1M=ÿ��ticks�����us��Ȼ�����50�͵ó�50us�����ticks
    led_encoder->reset_code = (rmt_symbol_word_t) {
        .level0 = 0,
        .duration0 = reset_ticks,
        .level1 = 0,
        .duration1 = reset_ticks,
    };

    //���ر�����
    *ret_encoder = &led_encoder->base;
    return ESP_OK;
err:
    if (led_encoder) {
        if (led_encoder->bytes_encoder) {
            rmt_del_encoder(led_encoder->bytes_encoder);
        }
        if (led_encoder->copy_encoder) {
            rmt_del_encoder(led_encoder->copy_encoder);
        }
        free(led_encoder);
    }
    return ret;
}

/** ��ʼ��WS2812����
 * @param gpio ����WS2812�Ĺܽ�
 * @param maxled ����WS2812�ĸ���
 * @param led_handle ���صĿ��ƾ��
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_init(gpio_num_t gpio,int maxled,ws2812_strip_handle_t* handle)
{
    struct ws2812_strip_t* led_handle = NULL;
    //����һ��WS2812��������
    led_handle = calloc(1, sizeof(struct ws2812_strip_t));
    assert(led_handle);
    //����led����������RGB��������
    led_handle->led_buffer = calloc(1,maxled*3);
    assert(led_handle->led_buffer);
    //����LED����
    led_handle->led_num = maxled;

    //����һ��RMT����ͨ������
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,         //Ĭ��ʱ��Դ
        .gpio_num = gpio,                       //GPIO�ܽ�
        .mem_block_symbols = 64,                //�ڴ���С���� 64 * 4 = 256 �ֽ�
        .resolution_hz = LED_STRIP_RESOLUTION_HZ,   //RMTͨ���ķֱ���10000000hz=0.1us��Ҳ���ǿ��Կ��Ƶ���Сʱ�䵥Ԫ
        .trans_queue_depth = 4,                 //�ײ��̨���͵Ķ������
    };

    //����һ��RMT����ͨ��
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_handle->led_chan));

    //�����Զ�����������ص㺯��������ν���룬���Ƿ������ʱ�������ǵ�ʱ�����
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&led_handle->led_encoder));

    //ʹ��RMTͨ��
    ESP_ERROR_CHECK(rmt_enable(led_handle->led_chan));

    //����WS2812�������
    *handle = led_handle;

    return ESP_OK;
}

/** ����ʼ��WS2812����
 * @param handle ��ʼ���ľ��
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_deinit(ws2812_strip_handle_t handle)
{
    if(!handle)
        return ESP_OK;
    rmt_del_encoder(handle->led_encoder);
    if(handle->led_buffer)
        free(handle->led_buffer);
    free(handle);
    return ESP_OK;
}

/** ��ĳ��WS2812д��RGB����
 * @param handle ���
 * @param index �ڼ���WS2812��0��ʼ��
 * @param r,g,b RGB����
 * @return ESP_OK or ESP_FAIL
*/
esp_err_t ws2812_write(ws2812_strip_handle_t handle,uint32_t index,uint32_t r,uint32_t g,uint32_t b)
{
     rmt_transmit_config_t tx_config = {
        .loop_count = 0, //��ѭ������
    };
    if(index >= handle->led_num)
        return ESP_FAIL;
    uint32_t start = index*3;
    handle->led_buffer[start+0] = g & 0xff;     //ע�⣬WS2812������˳��ʱGRB
    handle->led_buffer[start+1] = r & 0xff;
    handle->led_buffer[start+2] = b & 0xff;

    return rmt_transmit(handle->led_chan, handle->led_encoder, handle->led_buffer, handle->led_num*3, &tx_config);
    
}



// esp_err_t ws2812_fade(ws2812_strip_handle_t handle, uint32_t start_index, uint32_t end_index, uint32_t start_r, uint32_t start_g, uint32_t start_b, uint32_t end_r, uint32_t end_g, uint32_t end_b, uint32_t steps, uint32_t delay_ms)
// {
//     if (start_index >= handle->led_num || end_index >= handle->led_num || start_index > end_index) {
//         return ESP_FAIL; // ��Ч������
//     }

//     uint32_t total_leds = end_index - start_index + 1;
//     if (total_leds == 0) {
//         return ESP_OK; // û�� LED ��Ҫ����
//     }

//     // ����ÿһ������ɫ����
//     float step_r, step_g, step_b;
//     step_r = (float)(end_r - start_r) / (steps - 1);
//     step_g = (float)(end_g - start_g) / (steps - 1);
//     step_b = (float)(end_b - start_b) / (steps - 1);

//     for (uint32_t step = 0; step < steps; ++step) {
//         for (uint32_t i = start_index; i <= end_index; ++i) {
//             uint32_t current_r = (uint32_t)(start_r + step * step_r);
//             uint32_t current_g = (uint32_t)(start_g + step * step_g);
//             uint32_t current_b = (uint32_t)(start_b + step * step_b);

//             // ȷ����ɫֵ�ںϷ���Χ��
//             current_r = (current_r > 255) ? 255 : (current_r < 0 ? 0 : current_r);
//             current_g = (current_g > 255) ? 255 : (current_g < 0 ? 0 : current_g);
//             current_b = (current_b > 255) ? 255 : (current_b < 0 ? 0 : current_b);

//             // д����ɫ����
//             ws2812_write(handle, i, current_r, current_g, current_b);
//         }
//         vTaskDelay(pdMS_TO_TICKS(delay_ms)); // �ȴ�һ��ʱ�䣬�Ա�۲콥��Ч��
//     }

//     // ȷ������״̬
//     for (uint32_t i = start_index; i <= end_index; ++i) {
//         ws2812_write(handle, i, end_r, end_g, end_b);
//     }

//     return ESP_OK;
// }


esp_err_t ws2812_fade(ws2812_strip_handle_t handle, uint32_t start_index, uint32_t end_index, uint32_t start_r, uint32_t start_g, uint32_t start_b, uint32_t end_r, uint32_t end_g, uint32_t end_b, uint32_t steps, uint32_t delay_ms)
{
    if (start_index >= handle->led_num || end_index >= handle->led_num || start_index > end_index) {
        return ESP_FAIL;
    }

    uint32_t total_leds = end_index - start_index + 1;
    if (total_leds == 0) {
        return ESP_OK;
    }

    // ʹ����������ÿһ�������������⾫�ȶ�ʧ
    int32_t delta_r = (int32_t)(end_r - start_r);
    int32_t delta_g = (int32_t)(end_g - start_g);
    int32_t delta_b = (int32_t)(end_b - start_b);

    for (uint32_t step = 0; step < steps; ++step) {
        uint32_t current_r = start_r + (step * delta_r) / (steps - 1);
        uint32_t current_g = start_g + (step * delta_g) / (steps - 1);
        uint32_t current_b = start_b + (step * delta_b) / (steps - 1);

        for (uint32_t i = start_index; i <= end_index; ++i) {
            ws2812_write(handle, i, current_r, current_g, current_b);
        }
        vTaskDelay(pdMS_TO_TICKS(delay_ms));
    }

    // ȷ������״̬
    for (uint32_t i = start_index; i <= end_index; ++i) {
        ws2812_write(handle, i, end_r, end_g, end_b);
    }

    return ESP_OK;
}
