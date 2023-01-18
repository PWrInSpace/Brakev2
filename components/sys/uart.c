// Copyright 2022 PWrInSpace

#include "uart.h"

bool UART_init(uart_t *uart, uart_port_t port, uint8_t tx_pin,
               uint8_t rx_pin, uint32_t baudrate) {
    esp_err_t res;
    uart_config_t uart_conf = {
        .baud_rate = baudrate,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
        .source_clk = UART_SCLK_DEFAULT,
    };
    *uart = port;
    res = uart_param_config(*uart, &uart_conf);
    if (res != ESP_OK) {
        return false;
    }

    res = uart_set_pin(*uart, rx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    if (res != ESP_OK) {
        return false;
    }

    res = uart_driver_install(*uart, RX_BUFFER_SIZE, TX_BUFFER_SIZE, 0, NULL, 0);
    if (res != ESP_OK) {
        return false;
    }

    return true;
}

int UART_read(uart_t *uart, char *buffer, size_t buffer_size, TickType_t wait) {
    int length = uart_read_bytes(*uart, buffer, buffer_size, wait);

    if (length >= buffer_size) {
        length = buffer_size;
        buffer[length - 1] = '\0';
    }

    buffer[length] = '\0';

    return length;
}

int UART_read_until(uart_t *uart, char *buffer, size_t buffer_size,
                    char sign, TickType_t timeout) {
    char temp_buff = 0xFF;
    int read_bytes = 0;
    int read_size;
    TickType_t start_time = xTaskGetTickCount();
    while (temp_buff != sign) {
        read_size = uart_read_bytes(*uart, &temp_buff, sizeof(temp_buff), pdMS_TO_TICKS(2));
        if (read_size > 0) {
            // start_time = xTaskGetTickCount();
            buffer[read_bytes] = temp_buff;
            read_bytes++;
        }

        if (xTaskGetTickCount() - start_time > timeout) {
            break;
        }
    }

    if (read_bytes > 0) {
        buffer[read_bytes++] = '\0';
    }

    return read_bytes;
}

bool UART_flush_rx(uart_t *uart) {
    esp_err_t res;
    res = uart_flush(*uart);
    return res == ESP_OK ? true : false;
}


bool UART_flush_tx(uart_t *uart) {
    esp_err_t res;
    res = uart_flush_input(*uart);
    return res == ESP_OK ? true : false;
}
