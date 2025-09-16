#include "dist_sensor.h"
#include "pico/stdlib.h"

#include <sstream>
#include <iomanip>


DistSensor::DistSensor(int trigger_pin, int echo_pin) {
    this->trigger_pin = trigger_pin;
    this->echo_pin = echo_pin;

    gpio_init(this->trigger_pin);
    gpio_set_dir(this->trigger_pin, GPIO_OUT);

    gpio_init(this->echo_pin);
    gpio_set_dir(this->echo_pin, GPIO_IN);
}

float DistSensor::measure() {
    
    // Creates a soundwave.
    gpio_put(this->trigger_pin, 1);
    busy_wait_us_32(10);
    gpio_put(this->trigger_pin, 0);

    // Record time until reach to sensor again after hitting an object
    while(!gpio_get(this->echo_pin)) {
        busy_wait_us_32(1);
    }

    uint32_t start_time = time_us_64();

    while( (time_us_64() - start_time) * SPEED_OF_SOUND < MAX_DIST * 2 &&
          gpio_get(this->echo_pin)) {

        busy_wait_us_32(1);
    }

    uint32_t end_time = time_us_64();

    // Time until reaching is half of total time rec.
    float delta_t = (end_time - start_time) / 2;

    float dist = SPEED_OF_SOUND * delta_t;

    return dist;
}

std::string DistSensor::measure_to_str(int precision, std::string prefix) {
    float dist = this->measure();
    std::ostringstream s;
    s << prefix <<std::fixed << std::setprecision(precision) << dist;
    return s.str();
}