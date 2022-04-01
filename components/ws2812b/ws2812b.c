#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"

static const char *TAG = "ws2812b";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)
static led_strip_t *strip;
static int m_number_led =0;
/**
 * @brief Simple helper function, converting HSV color space to RGB color space
 *
 * Wiki: https://en.wikipedia.org/wiki/HSL_and_HSV
 *
 */
void ws2812_init(int pin, int number_led){
    m_number_led = number_led;
      rmt_config_t config = RMT_DEFAULT_CONFIG_TX(pin, RMT_TX_CHANNEL);
    // set counter clock to 40MHz
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    // install ws2812 driver
    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(number_led, (led_strip_dev_t)config.channel);
    strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
}

void ws2812_set_rgb(int index, uint8_t r, uint8_t g, uint8_t b){
    ESP_ERROR_CHECK(strip->set_pixel(strip, index, r, g, b));
    ESP_ERROR_CHECK(strip->refresh(strip, 100)); 
}

void ws2812_turnOff(){
    ESP_ERROR_CHECK(strip->clear(strip, 100)); 
}

void ws2812_set_all_rgb(uint8_t r, uint8_t g, uint8_t b){
    for(int i=0; i < m_number_led; i++){
        ESP_ERROR_CHECK(strip->set_pixel(strip, i, r, g, b));
    }
    ESP_ERROR_CHECK(strip->refresh(strip, 100)); 
}