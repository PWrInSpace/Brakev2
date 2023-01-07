// Copyright 2022 PWrInSpace

#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

#include <stdbool.h>
#include "driver/uart.h"

#define RX_BUFFER_SIZE 2048
#define TX_BUFFER_SIZE 2048

typedef uart_port_t uart_t;

bool UART_init(uart_t *uart, uart_port_t port, uint8_t tx_pin,
               uint8_t rx_pin, uint32_t baudrate);
int UART_read(uart_t *uart, char *buffer, size_t buffer_size, TickType_t wait);
int UART_read_until(uart_t *uart, char *buffer, size_t buffer_size,
                    char sign, TickType_t tiemout);
bool UART_flush_rx(uart_t *uart);
bool UART_flush_tx(uart_t *uart);
#endif
