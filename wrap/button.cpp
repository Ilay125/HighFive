#include "button.h"
#include "pico/stdlib.h"

/**
 * @fn Button
 * @brief Construct a new Button object.
 * @param pin GPIO pin number where the button is connected.
 * @param toggle true if the button is a toggle button, false otherwise.
 * @param pulldir 'u' for pull-up resistor, 'd' for pull-down. Default is 'u'.
 */
Button::Button(int pin, bool toggle, char pulldir) {
    this->pin = pin;
    this->toggle = toggle;
    this->pulldir = pulldir;

    if (this->pulldir != 'd') {
        this->pulldir = 'u';
    }
    
    this->toggle_state = false;
    this->_last_pressed = false;

    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);

    if (pulldir == 'd') {
        gpio_pull_down(pin); 
    } else {
        gpio_pull_up(pin);
    } 
}

/**
 * @fn is_pressed
 * @brief Check if the button is pressed.
 * @return true if the button is pressed, false otherwise.
 */
bool Button::is_pressed() {
    bool pressed = gpio_get(pin);
    pressed = (pulldir == 'u') ? !pressed : pressed; // invert if pull-up

    bool toggle_flag = pressed && !this->_last_pressed;

    if (toggle_flag) {
        this->debounce();
    }

    if (this->toggle) {
        if (toggle_flag) {
            this->toggle = !this->toggle;
        }

        this->_last_pressed = pressed;
        return this->toggle;
    }

    this->_last_pressed = pressed;
    return pressed;
}

void Button::debounce() {
    bool state = gpio_get(pin);
    sleep_ms(10); // Wait for bounce to settle
    while (gpio_get(pin) == state) {
        sleep_ms(10);
    }
}

