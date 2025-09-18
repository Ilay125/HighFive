#include <stdio.h>
#include <cmath>
#include <string>
#include "pico/stdlib.h"

#include "wrap/servo.h"
#include "wrap/debug.h"
#include "wrap/button.h"
#include "wrap/comm.h"
#include "wrap/dist_sensor.h"

#define SERVO_DOWN_PIN 0
#define SERVO_UP_PIN 2
#define BUTTON_PIN 7

#define US_TRIG_PIN 14
#define US_ECHO_PIN 15

#define SERVO_FREQ 50 // Standard servos run at 50Hz

#define PEN_LEN 13 // in cm

#define UART_TX_PIN 20
#define UART_RX_PIN 21

void rest_mode(Servo& up, Servo& down) {
    up.set_angle(15, false);
    down.set_angle(0, false);
}

int main()
{
    stdio_usb_init();    

    Debug debug;
    debug.blink(3, 200);

    Servo servo_down(SERVO_DOWN_PIN, SERVO_FREQ); 
    Servo servo_up(SERVO_UP_PIN, SERVO_FREQ);

    rest_mode(servo_up, servo_down);

    DistSensor dist(US_TRIG_PIN, US_ECHO_PIN);

    Comm comm(UART_TX_PIN, UART_RX_PIN, 1);
    printf("STARTING COMMS\n");
    
    printf("STARTING A LOOP\n");
    // MAIN LOOP - LISTENER
    std::vector<std::string> data;

    while (1) {
        std::string line;
        int get_msg_code = comm.get_msg(line);

        // ERROR
        if (get_msg_code == 2) {
            printf("NOOOOOO\n");
            debug.blink(5, 200);
            continue;
        }

        // GOT A MSG
        if (get_msg_code == 1) {
            printf("\nGOT A MSG!! %s\n", line.c_str());

            comm.split(line, ',', data);

            int up_angle = std::stoi(data.at(0));
            int down_angle = std::stoi(data.at(1));
            int is_fast = std::stoi(data.at(2));

            servo_up.set_angle(up_angle, is_fast);
            servo_down.set_angle(down_angle, is_fast);

            data.clear();

            continue;
        }

        // NOTHING

        
        std::string dist_str = dist.measure_to_str(3, "us"); 

        printf("sending %s\n", dist_str.c_str());
        comm.send_msg(dist_str);

        float l = dist.measure();

        if (l >= 25) {
            rest_mode(servo_up, servo_down);
        } else {
            float alpha_f = asin(l / (2 * PEN_LEN)) * 180 / 3.14;
            int alpha = round(alpha_f);
            
            servo_down.set_angle(alpha);
            servo_up.set_angle(2 * alpha);
            sleep_ms(1000);
        }

        sleep_ms(500);

    }

    return 0;
}
