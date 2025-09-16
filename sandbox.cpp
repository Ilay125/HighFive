#include <stdio.h>
#include <cmath>
#include <string>
#include "pico/stdlib.h"

#include "wrap/servo.h"
#include "wrap/debug.h"
#include "wrap/button.h"
#include "wrap/comm.h"

#define SERVO_DOWN_PIN 0
#define SERVO_UP_PIN 2
#define BUTTON_PIN 7

#define US_TRIG_PIN 14
#define US_ECHO_PIN 15

#define SERVO_FREQ 50 // Standard servos run at 50Hz

#define PEN_LEN 13 // in cm

#define UART_TX_PIN 16
#define UART_RX_PIN 17

void rest_mode(Servo& up, Servo& down) {
    up.set_angle(15, false);
    down.set_angle(0, false);
    sleep_ms(1000);
}

int main()
{
    stdio_init_all();    

    Debug debug;
    debug.blink(3, 200);

    Servo servo_down(SERVO_DOWN_PIN, SERVO_FREQ); 
    Servo servo_up(SERVO_UP_PIN, SERVO_FREQ);

    rest_mode(servo_up, servo_down);

    Comm comm(UART_TX_PIN, UART_RX_PIN, 0);
    printf("STARTING COMMS\n");
    
    printf("STARTING A LOOP\n");
    // MAIN LOOP - LISTENER
    std::vector<std::string> data;

    comm.send_msg("6969");

    while (1) {
        std::string line;
        if (comm.get_msg(line)) {
            printf("NOOOOOO\n");
            debug.blink(5, 200);
            continue;
        }

        printf("\nGOT A MSG!! %s\n", line.c_str());

        comm.send_msg("6969");
        
        comm.split(line, ',', data);

        int up_angle = std::stoi(data.at(0));
        int down_angle = std::stoi(data.at(1));
        int is_fast = std::stoi(data.at(2));

        servo_up.set_angle(up_angle, is_fast);
        servo_down.set_angle(down_angle, is_fast);

        data.clear();
    }



    
    return 0;
}
