#include "comm.h"
#include "pico/stdlib.h"

Comm::Comm(int tx_pin, int rx_pin, int uart_num) {
    this->uart_channel = !uart_num ? uart0 : uart1;

    uart_init(this->uart_channel, BAUDRATE);

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
    if (!uart_is_readable(this->uart_channel)) {
        return 0;
    }

    int c = uart_getc(this->uart_channel);

    while (this->buf_idx < BUFFER_SIZE - 1 && c != ENDLINE) {
        if (c == EOF || c == PICO_ERROR_TIMEOUT) {
            return 2;
        }

        this->buffer[this->buf_idx++] = c;
        
        c = uart_getc(this->uart_channel);
    }

    if (this->buf_idx >= BUFFER_SIZE - 1) {
        this->clean_buffer();
        return 2;
    }

    this->buffer[this->buf_idx] = '\0';

    std::string buf_str(this->buffer);
    data = buf_str;

    this->clean_buffer();

    return 1;
}

void Comm::send_msg(std::string data) {
    data += ENDLINE;
    data += '\n';
    uart_puts(this->uart_channel, data.c_str());
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