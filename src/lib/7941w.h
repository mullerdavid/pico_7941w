#include <stdbool.h>
#include "hardware/uart.h"

void rfid_7941w_init(uart_inst_t * uart);
void rfid_7941w_init_generic(uart_inst_t * uart, uint tx_pin, uint rx_pin);
void rfid_7941w_send(uart_inst_t *uart, uint8_t address, uint8_t command, uint8_t length, uint8_t *data);
bool rfid_7941w_recv(uart_inst_t *uart, uint8_t *address, uint8_t *command, uint8_t *length, uint8_t *data);
