#include <stdbool.h>
#include "hardware/uart.h"

void rfid_7941w_init(uart_inst_t * uart);

void rfid_7941w_init_generic(uart_inst_t * uart, uint tx_pin, uint rx_pin);

// data is optional, can be NULL if length is 0
void rfid_7941w_send(uart_inst_t *uart, uint8_t address, uint8_t command, uint8_t length, uint8_t *data);

// data must have enough space for 255 bytes, returns true on success
bool rfid_7941w_recv(uart_inst_t *uart, uint8_t *address, uint8_t *command, uint8_t *length, uint8_t *data);

// read 125 kHz, eg T5577
bool rfid_7941w_read_LF(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// read 13.56 MHz, eg RFID
bool rfid_7941w_read_HF(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// try to read HF first then LF
bool rfid_7941w_read(uart_inst_t *uart, uint8_t *length, uint8_t *data);

// write 125 kHz, eg T5577
bool rfid_7941w_write_LF(uart_inst_t *uart, uint8_t length, uint8_t *data);

// write 13.56 MHz, eg RFID
bool rfid_7941w_write_HF(uart_inst_t *uart, uint8_t length, uint8_t *data);

typedef enum rfid_7941w_type {
  LF = 0b01,
  HF = 0b10,
  ERROR = -1,
} rfid_7941w_type_t;


// T5577 id has 5 bytes, RFID has 4 bytes

// try to read HF first then LF, returns uint64
uint64_t rfid_7941w_alt_read(uart_inst_t *uart);

// try to read HF first then LF, returns uint64
uint64_t rfid_7941w_alt_read_with_info(uart_inst_t *uart, rfid_7941w_type_t *info);

// write 125 kHz, eg T5577, uint parameters
bool rfid_7941w_alt_write_LF(uart_inst_t *uart, uint8_t vendor, uint32_t id);

// write 13.56 MHz, eg RFID, uint parameters
bool rfid_7941w_alt_write_HF(uart_inst_t *uart, uint32_t id);