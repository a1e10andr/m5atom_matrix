#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_err.h"

typedef struct structDisplayMessage
{
    char message[20];    
} DisplayMessage_t;

/**
* @brief M5Atom Matrix Type
*
*/
typedef struct m5atom_matrix_s m5atom_matrix_t;


/**
* @brief M5Atom Matrix Device Type
*
*/
typedef void *m5atom_matrix_dev_t;

/**
* @brief Declare of M5 Atom Matrix Type
*
*/
struct m5atom_matrix_s {
    /**
    * @brief Initialize M5 Atom Matrix core functions
    *
    * @param atom:  M5 Atom matrix instance
    *
    * @return
    *      - ESP_OK: Initialization success
    *      - ESP_FAIL: Initialization failed because other error occurred
    */
    esp_err_t (*initialize)(m5atom_matrix_t *atom);
}
//     /**
//     * @brief Set RGB for a specific pixel
//     *
//     * @param strip: LED strip
//     * @param index: index of pixel to set
//     * @param red: red part of color
//     * @param green: green part of color
//     * @param blue: blue part of color
//     *
//     * @return
//     *      - ESP_OK: Set RGB for a specific pixel successfully
//     *      - ESP_ERR_INVALID_ARG: Set RGB for a specific pixel failed because of invalid parameters
//     *      - ESP_FAIL: Set RGB for a specific pixel failed because other error occurred
//     */
//     esp_err_t (*set_pixel)(led_strip_t *strip, uint32_t index, uint32_t red, uint32_t green, uint32_t blue);

//     /**
//     * @brief Refresh memory colors to LEDs
//     *
//     * @param strip: LED strip
//     * @param timeout_ms: timeout value for refreshing task
//     *
//     * @return
//     *      - ESP_OK: Refresh successfully
//     *      - ESP_ERR_TIMEOUT: Refresh failed because of timeout
//     *      - ESP_FAIL: Refresh failed because some other error occurred
//     *
//     * @note:
//     *      After updating the LED colors in the memory, a following invocation of this API is needed to flush colors to strip.
//     */
//     esp_err_t (*refresh)(led_strip_t *strip, uint32_t timeout_ms);

//     /**
//     * @brief Clear LED strip (turn off all LEDs)
//     *
//     * @param strip: LED strip
//     * @param timeout_ms: timeout value for clearing task
//     *
//     * @return
//     *      - ESP_OK: Clear LEDs successfully
//     *      - ESP_ERR_TIMEOUT: Clear LEDs failed because of timeout
//     *      - ESP_FAIL: Clear LEDs failed because some other error occurred
//     */
//     esp_err_t (*clear)(led_strip_t *strip, uint32_t timeout_ms);

//     /**
//     * @brief Free LED strip resources
//     *
//     * @param strip: LED strip
//     *
//     * @return
//     *      - ESP_OK: Free resources successfully
//     *      - ESP_FAIL: Free resources failed because error occurred
//     */
//     esp_err_t (*del)(led_strip_t *strip);
// };

/**
* @brief M5Atom Matrix Configuration Type
*
*/
typedef struct {
    m5atom_matrix_dev_t dev; /*!< M5 atom matrix device */
} m5atom_matrix_config_t;

// /**
//  * @brief Default configuration for LED strip
//  *
//  */
// #define LED_STRIP_DEFAULT_CONFIG(number, dev_hdl, gpio) \
//     {                                             \
//         .max_leds = number,                       \
//         .dev = dev_hdl,                           \
//         .gpio = gpio,                             \
//     }

// /**
// * @brief Install a new ws2812 driver (based on RMT peripheral)
// *
// * @param config: LED strip configuration
// * @return
// *      LED strip instance or NULL
// */
// led_strip_t *led_strip_new_rmt_ws2812(const led_strip_config_t *config);

// /**
//  * @brief Init the RMT peripheral and LED strip configuration.
//  *
//  * @param[in] channel: RMT peripheral channel number.
//  * @param[in] gpio: GPIO number for the RMT data output.
//  * @param[in] led_num: number of addressable LEDs.
//  * @return
//  *      LED strip instance or NULL
//  */
// led_strip_t * led_strip_init(uint8_t channel, uint8_t gpio, uint16_t led_num);

// /**
//  * @brief Denit the RMT peripheral.
//  *
//  * @param[in] strip: LED strip
//  * @return
//  *     - ESP_OK
//  *     - ESP_FAIL
//  */
// esp_err_t led_strip_denit(led_strip_t *strip);

#ifdef __cplusplus
}
#endif
