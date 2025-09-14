#ifndef __BUTTON__
#define __BUTTON__

class Button {
    int pin;
    bool toggle;
    char pulldir; // 'u' for pullup, 'd' for pulldown

    bool toggle_state; // current state for toggle buttons
    bool _last_pressed; 

    public:
        /**
         * @fn Button
         * @brief Construct a new Button object.
         * @param pin GPIO pin number where the button is connected.
         * @param toggle true if the button is a toggle button, false otherwise.
         * @param pulldir 'u' for pull-up resistor, 'd' for pull-down. Default is 'u'.
         */
        Button(int pin, bool toggle, char pulldir='u');

        /**
         * @fn is_pressed
         * @brief Check if the button is pressed.
         * @return true if the button is pressed, false otherwise.
         */
        bool is_pressed();

        /**
         * @fn debounce
         * @brief Debounce the button press.
         */
        void debounce();
};

#endif