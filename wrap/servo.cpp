#include <cmath>
#include "servo.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"


#define MIN_PULSE_WIDTH_PER 0.025
#define MAX_PULSE_WIDTH_PER 0.125

#define SLOW_INCREMENT 5

Servo::Servo(int pin, int f) {
    this->pin = pin;
    this->angle = 0;
    this->freq = f;
    
    gpio_set_function(this->pin, GPIO_FUNC_PWM);

    int slice_num = pwm_gpio_to_slice_num(this->pin);
    this->channel = pwm_gpio_to_channel(this->pin);

    this->slice = new Slice(slice_num, this->freq, true);
}

int Servo::get_pin() {
    return this->pin;
}

int Servo::get_angle() {
    return this->angle;
}

void Servo::_set_angle_fast(int angle) {
    this->angle = angle;

    int max_pulse_width = this->slice->get_wrap() * MAX_PULSE_WIDTH_PER;
    int min_pulse_width = this->slice->get_wrap() * MIN_PULSE_WIDTH_PER;

    int rel_time = (this->angle * (max_pulse_width - min_pulse_width)) / 180;
    int pulse_width = min_pulse_width + rel_time;

    pwm_set_chan_level(this->slice->get_num(), this->channel, pulse_width);
}

void Servo::_set_angle_slow(int angle) {
    int current = this->angle;

    int step = (current < angle) ? SLOW_INCREMENT : -SLOW_INCREMENT;

    while (abs(angle - current) > SLOW_INCREMENT) {
        current += step;
        this->_set_angle_fast(current);
        sleep_ms(500);  // adjust for speed
    }

    // Sets final angle exactly
    this->_set_angle_fast(angle);
}

void Servo::set_angle(int angle, bool fast) {
    if (angle < 0) {
        angle = 0;
    } else if (angle > 180) {
        angle = 180;
    }

    this->angle = angle;

    if (fast) {
        this->_set_angle_fast(angle);
    } else {
        this->_set_angle_slow(angle);
    }
}

void Servo::add_angle(int angle, bool fast) {
    this->set_angle(this->angle + angle, fast);
}

Servo::~Servo() {
    delete this->slice;
}