//
// Created by Charles on 11/08/2024.
//

#include "system.hpp"

#include <hardware.hpp>
#include <iostream>

#include "color.hpp"
#include "graphics.hpp"

void libsystem::delay(uint64_t ms) {

#ifdef ESP_PLATFORM
    vTaskDelay(pdMS_TO_TICKS(ms));
#else
    SDL_Delay(ms);
#endif

}

void libsystem::panic(const std::string &message) {
    LGFX* lcd = graphics::getLGFX();

    std::cerr << "System panicked !" << std::endl;
    std::cerr << "- OS Version: " << OS_VERSION << std::endl;
    std::cerr << "- Message: " << message << std::endl;

    // Reset configuration
    lcd->clear(graphics::packRGB565(0, 0, 0));
    lcd->setFont(&DejaVu12);
    lcd->setCursor(0, 0);
    lcd->setTextColor(graphics::packRGB565(255, 255, 255));

    // Print OS name
    lcd->setFont(&DejaVu40);
    lcd->setTextColor(graphics::packRGB565(58, 186, 153));
    lcd->printf("%s\n", OS_NAME);
    lcd->setFont(&DejaVu12);
    lcd->setTextColor(graphics::packRGB565(255, 255, 255));

    // Print OS version
    lcd->printf("\nOS Version : %s\n\n", OS_VERSION);

    // Show error message
    lcd->printf("\n\nSystem Panic:\n");
    lcd->setTextColor(graphics::packRGB565(255, 100, 100));
    lcd->printf("    %s\n\n\n", message.c_str());
    lcd->setTextColor(graphics::packRGB565(255, 255, 255));

    // Show instructions
    lcd->printf("This is not an expected behavior, please report this to the Paxo / PaxOS team.\n\n");
    lcd->printf("Contact us :\n- https://paxo.fr/\n\n");
    lcd->printf("What you should do:\n");
    lcd->printf("- Report this issue with every possible detail (what you done, installed applications...).\n");
    lcd->printf("- Check and clean the SD Card.\n");
    lcd->printf("- Re-flash this device.\n");

    // Show hint
    lcd->printf("\n\nThe device will restart in 15 seconds.");

    // Vibrate to alert user
    for (uint8_t i = 0; i < 3; i++) {
        hardware::setVibrator(true);
        delay(100);
        hardware::setVibrator(false);
        delay(200);
    }

    // Wait sometime to allow the user to read the message
    delay(15000);

    // Make the system crash (and restart)
    throw std::runtime_error("System panic: " + message);
}

void libsystem::log(const std::string &message) {
    std::cout << "[LOG] " << message << std::endl;
}

libsystem::exceptions::RuntimeError::RuntimeError(const std::string &message): runtime_error(message) {
    panic(message);
}

libsystem::exceptions::RuntimeError::RuntimeError(const char* message): runtime_error(message) {
    panic(message);
}

libsystem::exceptions::OutOfRange::OutOfRange(const std::string &message): out_of_range(message) {
    panic(message);
}

libsystem::exceptions::OutOfRange::OutOfRange(const char *message): out_of_range(message) {
    panic(message);
}
