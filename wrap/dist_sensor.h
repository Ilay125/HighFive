#ifndef __DIST_SENSOR__
#define __DIST_SENSOR__

#define SPEED_OF_SOUND 34300 // [cm/s] 
#define MAX_DIST 25 // [cm]

class DistSensor {
    int trigger_pin;
    int echo_pin;

    public:
        DistSensor(int trigger_pin, int echo_pin);
        float measure();
};

#endif