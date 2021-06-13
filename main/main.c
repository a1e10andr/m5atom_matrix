/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "nvs_flash.h"
#include "driver/rmt.h"
#include "driver/gpio.h"
#include "led_strip.h"

#define RMT_TX_CHANNEL                      RMT_CHANNEL_0
#define STRIP_LED_NUMBER                    25
#define RMT_TX_GPIO                         GPIO_NUM_27

#define COLOR_LEVEL                         25

#define STACK_SIZE                          2048    //Task stack size

static QueueHandle_t ws2812QueueHandle = NULL;

typedef struct structWS2812Message
 {
    uint8_t index;
    uint8_t r;
    uint8_t g;
    uint8_t b;
 } WS2812Message_t;

typedef enum enumState {
  red = 0,
  green = 1,
  blue = 2,
} state_t;

typedef state_t (*stateFunction_t)(WS2812Message_t *);

static uint8_t digits[10][15] = {
    {0, 1, 0, 
     1, 0, 1,
     1, 0, 1,
     1, 0, 1,
     0, 1, 0},
    {0, 1, 0, 
     1, 1, 0,
     0, 1, 0,
     0, 1, 0, 
     1, 1, 1},
    {1, 1, 0, 
     0, 0, 1,
     0, 1, 0,
     1, 0, 0, 
     1, 1, 1},
    {1, 1, 0, 
     0, 0, 1,
     0, 1, 0,
     0, 0, 1, 
     1, 1, 1},
    {1, 0, 1, 
     1, 0, 1,
     1, 1, 0,
     0, 0, 1, 
     0, 0, 1},
    {0, 1, 1, 
     1, 0, 0,
     1, 1, 0,
     0, 0, 1, 
     1, 1, 0},
    {0, 1, 1, 
     1, 0, 0,
     1, 1, 0,
     1, 0, 1, 
     1, 1, 0},
    {1, 1, 1, 
     0, 0, 1,
     0, 1, 0,
     0, 1, 0, 
     0, 1, 0},
    {0, 1, 0, 
     1, 0, 1,
     0, 1, 0,
     1, 0, 1, 
     0, 1, 0},
    {0, 1, 0, 
     1, 0, 1,
     0, 1, 1,
     0, 0, 1, 
     1, 1, 0},
};

static state_t ws2812_led_task__state__red(WS2812Message_t *msg) {
    msg->r = 0;
    msg->g = COLOR_LEVEL;
    return green;    
}

static state_t ws2812_led_task__state__green(WS2812Message_t *msg) {
    msg->g = 0;
    msg->b = COLOR_LEVEL;
    return blue;    
}

static state_t ws2812_led_task__state__blue(WS2812Message_t *msg) {
    msg->b = 0;
    msg->r = COLOR_LEVEL;
    return red;    
}

void ws2812_led_task(void *pvParameter)
{
    state_t state = red;
    stateFunction_t fn[3] = {
        ws2812_led_task__state__red,
        ws2812_led_task__state__green,
        ws2812_led_task__state__blue
    };

    WS2812Message_t msg = {
      .index = 0,
      .r = COLOR_LEVEL,
      .g = 0,
      .b = 0
    };

    for(;;) {
      if (msg.index == 0) {
        msg.index = STRIP_LED_NUMBER - 1;
        fn[state](&msg);
        /*
        switch (state)
        {
        case red:
          state = green;
          msg.r = 0;
          msg.g = COLOR_LEVEL;
          break;
        case green:
          state = blue;
          msg.g = 0;
          msg.b = COLOR_LEVEL;
          break;
        case blue:
          state = red;
          msg.b = 0;
          msg.r = COLOR_LEVEL;
          break;
        default:
          break;
        }
        */
      } else {
        msg.index -= 1;
      }
      xQueueSend(ws2812QueueHandle, (void *)&msg, (TickType_t)0);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
    vTaskDelete(NULL);
}

void ws2812_main_task(void *pvParameter)
{
    printf("Initialize WS2812 driver ...\n"); 
    led_strip_t *strip = led_strip_init(RMT_TX_CHANNEL, RMT_TX_GPIO, STRIP_LED_NUMBER);
    if (!strip) {
        printf("failed\n");
    }

    WS2812Message_t msg;
    for(;;) {
      if(xQueueReceive(ws2812QueueHandle, &msg, (TickType_t)1000)) {
        ESP_ERROR_CHECK(strip->set_pixel(strip,  msg.index, msg.r, msg.g, msg.b));
        ESP_ERROR_CHECK(strip->refresh(strip, 100));
      }
    }
    vTaskDelete(NULL);
}

void app_main(void)
{
    /* Print chip information */
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is ESP32 chip with %d CPU cores, WiFi%s%s, ",
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    nvs_flash_init();
    
    printf("Initialize GPIO ...\n");
    gpio_pad_select_gpio(RMT_TX_GPIO);
    ESP_ERROR_CHECK(gpio_set_direction(RMT_TX_GPIO, GPIO_MODE_OUTPUT));
    ESP_ERROR_CHECK(gpio_set_level(RMT_TX_GPIO, 0));

    ws2812QueueHandle = xQueueCreate(5, sizeof(WS2812Message_t));

    TaskHandle_t ws2812Handle = NULL;
    xTaskCreate(ws2812_main_task, "ws2812main", STACK_SIZE, NULL, tskIDLE_PRIORITY, &ws2812Handle);
    configASSERT(ws2812Handle);

    TaskHandle_t wsLedHandle = NULL;
    xTaskCreate(ws2812_led_task, "led", STACK_SIZE, NULL, tskIDLE_PRIORITY, &wsLedHandle);
    configASSERT(wsLedHandle);
}
