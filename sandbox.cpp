#include <stdio.h>
#include <cmath>
#include "pico/stdlib.h"

#include "wrap/servo.h"
#include "wrap/debug.h"
#include "wrap/button.h"

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
    up.set_angle(165);
    down.set_angle(0);
    sleep_ms(1000);
}

int main()
{
    stdio_init_all();

    printf("Welcom to my pico!\n\n");
    

    Debug debug;
    debug.blink(3, 200);

    /*

    Servo servo_down(SERVO_DOWN_PIN, SERVO_FREQ); 
    Servo servo_up(SERVO_UP_PIN, SERVO_FREQ);

    Button button(BUTTON_PIN, false);

    */

    uart_init(uart0, 115200);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    printf("STARTING COMMS\n");
    uart_puts(uart0, "HELLO FROM PICO|");
    sleep_ms(1000);

    printf("MSG SENT\n");
    while (1) {
        debug.blink(1,500);
        uart_puts(uart0, "ping!|");
    }

    printf("\nEND!");

    /*
    gpio_init(US_TRIG_PIN);
    gpio_set_dir(US_TRIG_PIN, GPIO_OUT);

    gpio_init(US_ECHO_PIN);
    gpio_set_dir(US_ECHO_PIN, GPIO_IN);

    rest_mode(servo_up, servo_down);
    
    sleep_ms(2000);
    
    while (1) {

        if (button.is_pressed()) {
            debug.set(1);
            rest_mode(servo_up, servo_down);
            sleep_ms(3000);
            debug.set(0);

            continue;
        }


        gpio_put(US_TRIG_PIN, 1);
        busy_wait_us_32(10);
        gpio_put(US_TRIG_PIN, 0);

        while(!gpio_get(US_ECHO_PIN)) {
            busy_wait_us_32(1);
        }

        uint32_t start_time = time_us_64();

        while(gpio_get(US_ECHO_PIN)) {
            busy_wait_us_32(1);
        }

        uint32_t end_time = time_us_64();

        uint32_t time_travel = end_time - start_time;

        float dist_cm_exact = time_travel / 2 * 0.034;

        int dist_cm = std::round(dist_cm_exact);

        dist_cm = dist_cm > 25 ? 25 : dist_cm;

        double angle_f = std::asin( dist_cm_exact / (2 * PEN_LEN) ) * 180 / 3.14;

        int angle = std::round(angle_f);

        printf("dist [cm]=%d, angle [deg]=%d\n", dist_cm, angle);


        
        servo_down.set_angle(angle);
        servo_up.set_angle(180 - 2 * angle);
        
        sleep_ms(500);

    }

    */
    
    
    
    return 0;
}
