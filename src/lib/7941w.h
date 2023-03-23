#include <stdbool.h>
#include "hardware/uart.h"

void rfid_7941w_init(uart_inst_t * uart);

void rfid_7941w_init_generic(uart_inst_t * uart, uint tx_pin, uint rx_pin);

// data is optional, can be NULL if length is 0
void rfid_7941w_send(uart_inst_t *uart, uint8_t address, uint8_t command, uint8_t length, uint8_t *data);

// data must have enough space for 255 bytes, returns true on success
bool rfid_7941w_recv(uart_inst_t *uart, uint8_t *address, uint8_t *command, uint8_t *length, uint8_t *data);

// 125 kHz, eg T5577
bool rfid_7941w_read_LF(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// 13.56 MHz, eg RFID
bool rfid_7941w_read_HF(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// try to read HF first then LF
bool rfid_7941w_read(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// 125 kHz, eg T5577
bool rfid_7941w_write_LF(uart_inst_t *uart, uint8_t length, uint8_t *data);

// 13.56 MHz, eg RFID
bool rfid_7941w_write_HF(uart_inst_t *uart, uint8_t length, uint8_t *data);
