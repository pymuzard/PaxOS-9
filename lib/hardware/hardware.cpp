#include "hardware.hpp"
#include "gpio.hpp"

#ifdef ESP_PLATFORM
    #include <Arduino.h>
    #include "driver/uart.h"
    #include "esp_system.h"
#else
    #ifdef __linux__
        #include <SDL2/SDL.h>
    #else
        #include <SDL_keyboard.h>
    #endif
#endif

void hardware::init()
{
    #ifdef ESP_PLATFORM

    psramInit();
    pinMode(PIN_SCREEN_POWER, OUTPUT);
    pinMode(PIN_VIBRATOR, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(PIN_HOME_BUTTON, INPUT_PULLUP);
    setVibrator(false);

    /*uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Set the source clock in the configuration
    #if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
        uart_config.source_clk = UART_SCLK_REF_TICK;
    #else
        uart_config.use_ref_tick = true;
    #endif

    // Configure UART0 (you can change to another UART if needed)
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_0, &uart_config));

    // Set UART pins (using default pins)
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    // Install UART driver
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_0, 256, 0, 0, NULL, 0));*/

    #endif
}

void hardware::setScreenPower(bool power)
{
    #ifdef ESP_PLATFORM

    digitalWrite(12, 0);
    digitalWrite(PIN_SCREEN_POWER, 0);

    delay(500);

    digitalWrite(PIN_SCREEN_POWER, power);

    delay(500);

    digitalWrite(12, 1);

    delay(500);

    digitalWrite(12, 0);

    delay(500);

    digitalWrite(12, 1);

    delay(500);
    #endif
}

void hardware::setVibrator(bool state)
{
    #ifdef ESP_PLATFORM

    digitalWrite(PIN_VIBRATOR, state);

    #endif
}

bool hardware::getHomeButton()
{
    #ifdef ESP_PLATFORM

        return !digitalRead(PIN_HOME_BUTTON);

    #else

        return (SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_ESCAPE] || SDL_GetKeyboardState(nullptr)[SDL_SCANCODE_Q]);
    
    #endif
}