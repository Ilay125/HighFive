#ifndef __DIST_SENSOR__
#define __DIST_SENSOR__

#include <string>


#define SPEED_OF_SOUND 0.0343 // [cm/us] 
#define MAX_DIST 25 // [cm]

class DistSensor {
    int trigger_pin;
    int echo_pin;

    public:
        DistSensor(int trigger_pin, int echo_pin);
        float measure();
        std::string measure_to_str(int precision, std::string prefix="");
};

#endif