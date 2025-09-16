#ifndef __COMM__
#define __COMM__

#include <string>
#include <vector>
#include "hardware/uart.h"

#define BUFFER_SIZE 65
#define BAUDRATE 115200

#define ENDLINE '|'

class Comm {
    char buffer[BUFFER_SIZE];
    int buf_idx;
    uart_inst_t* uart_channel;

    public:
        /**
         * @fn Comm
         * @brief initializing communication via UART. Requires 'stdio_init_all'.
         * @param tx_pin the GPIO pin of tx
         * @param rx_pin the GPIO pin of rx
         * @param uart_num channel num of uart. 0 on default.
         */
        Comm(int tx_pin, int rx_pin, int uart_num=0);

        /**
         * @fn get_msg
         * @brief receives a message and parse it to string.
         * @param[out] data a reference that will be updated with the new string.
         * @return error code.
         */
        int get_msg(std::string& data);

        void send_msg(std::string data);

        /**
         * @fn clean_buffer
         * @brief Resets buffer and index.
         */
        void clean_buffer();

        /**
         * @fn split
         * @brief splits a string into substrings between a common sep.
         *        And puts it inside a vector.
         * @param str the string to split
         * @param sep the seperator
         * @param[out] buffer a vector the will store the parts.
         */
        static void split(std::string str,
                      char sep,
                      std::vector<std::string>& buffer);
};

#endif