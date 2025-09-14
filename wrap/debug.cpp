#include "debug.h"
#include "pico/cyw43_arch.h"
#include <cstdlib>


Debug::Debug() {
    if (cyw43_arch_init()) { 
        exit(1); // failed to init Wi-Fi/LED driver
    }
}

void Debug::set(bool state) {
    this->state = state;
    cyw43_arch_gpio_put(DEBUG_LED, this->state);
}

bool Debug::get() {
    return this->state;
}

void Debug::toggle() {
    this->set(!this->state);
}

void Debug::blink(int times, int delay_ms) {
    for (int i = 0; i < times; i++) {
        this->toggle();
        sleep_ms(delay_ms);
        this->toggle();
        sleep_ms(delay_ms);
    }
}