#ifndef __DIST_SENSOR__
#define __DIST_SENSOR__

#include <string>


#define SPEED_OF_SOUND 0.0343 // [cm/us] 
#define MAX_DIST 25 // [cm]

class DistSensor {
    int trigger_pin;
    int echo_pin;

    public:
        /**
         * @fn DistSensor
         * @brief constructs a new distance sensor object.
         *        The distance sensor is based on ultrasonic sensor.
         * @param trigger_pin the PIN number of the 'trigger' - the input which
         *                    emits the US pulse.
         * @param echo_pin the PIN number of the 'echo' - the input which
         *                 sees the US pulse of the returning wave.
         */
        DistSensor(int trigger_pin, int echo_pin);

        /**
         * @fn measure
         * @brief measures the distance between an object and the sensor,
         *        with a limitation of max distance measurable in a const.
         * @return the distance, or MAX_DIST if dist is larger.
         */
        float measure();

        /**
         * @fn measure_to_str
         * @brief parse a string out of the rounded measurement,
         *        and concatinating a prefix if necessary. Used for comms.
         * @param precision the amount of digit after the decimal point.
         * @param prefix a prefix to add at the beginning of the string.
         * @return the parsed string.
         */
        std::string measure_to_str(int precision, std::string prefix="");
};

#endif