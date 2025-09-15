#include "comm.h"
#include "pico/stdlib.h"

Comm::Comm(int tx_pin, int rx_pin, int uart_num) {
    if (uart_num != 1) {
        uart_num = 0;
    }

    uart_init(uart_num ? uart1 : uart0, BAUDRATE);

    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);

    this->clean_buffer();
}

void Comm::clean_buffer() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        this->buffer[i] = 0;
    }

    this->buf_idx = 0;
}

int Comm::get_msg(std::string& data) {
    int c = getchar();

    while (this->buf_idx < BUFFER_SIZE - 1 && c != ENDLINE) {
        if (c == EOF || c == PICO_ERROR_TIMEOUT) {
            return 1;
        }

        this->buffer[this->buf_idx++] = c;

        c = getchar();
    }

    if (this->buf_idx >= BUFFER_SIZE - 1) {
        this->clean_buffer();
        return 1;
    }

    this->buffer[this->buf_idx] = '\0';

    std::string buf_str(this->buffer);
    data = buf_str;

    this->clean_buffer();

    return 0;
}

void Comm::split(std::string str, char sep, std::vector<std::string>& buffer) {
    if(!str.length()) {
        return;
    }

    size_t sep_idx = str.find(sep);
    while (sep_idx != std::string::npos) {
        std::string cut = str.substr(0, sep_idx);
        buffer.push_back(cut);
        str = str.substr(sep_idx + 1);
        sep_idx = str.find(sep);
    }

    buffer.push_back(str);
}