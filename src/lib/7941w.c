#include "7941w.h"

#include <string.h>
#include "hardware/gpio.h"
#include "hardware/uart.h"

// 7941W communicates with these
#define rfid_7941W_UART_BAUD 115200
#define rfid_7941W_UART_DATA_BITS 8
#define rfid_7941W_UART_STOP_BITS 1
#define rfid_7941W_UART_PARITY UART_PARITY_NONE

// response time ms
#define rfid_7941W_RESPONSE_TIME_MS 20

void rfid_7941w_init(uart_inst_t *uart)
{
    if (uart==uart0)
    {
        rfid_7941w_init_generic(uart0, 0, 1);
    }
    else if (uart==uart1)
    {
        rfid_7941w_init_generic(uart1, 4, 5);
    }
    else //invalid hw uart
    {
        assert(false);
    }
}

void rfid_7941w_init_generic(uart_inst_t *uart, uint tx_pin, uint rx_pin)
{
    uart_init(uart, 2400);
    gpio_set_function(tx_pin, GPIO_FUNC_UART);
    gpio_set_function(rx_pin, GPIO_FUNC_UART);
    int __unused actual = uart_set_baudrate(uart, rfid_7941W_UART_BAUD);
    uart_set_hw_flow(uart, false, false);
    uart_set_format(uart, rfid_7941W_UART_DATA_BITS, rfid_7941W_UART_STOP_BITS, rfid_7941W_UART_PARITY);
}

uint8_t calc_xor(uint8_t length, uint8_t *data) 
{
    uint8_t ret = 0x0;
    uint8_t * tail = data + length;
    while (data<tail)
    {
        ret = ret ^ (*data);
        data++;
    }
    return ret;
}

bool uart_read_blocking_timout(uart_inst_t *uart, uint8_t *dst, size_t len)
{
    const uint32_t bytes_per_sec = rfid_7941W_UART_BAUD / (rfid_7941W_UART_DATA_BITS + rfid_7941W_UART_STOP_BITS + 1);
    const uint32_t wait_per_byte_us = ( 1000000 + bytes_per_sec ) / bytes_per_sec + 10; // ceiling + little extra
    bool readable = uart_is_readable_within_us(uart, len*wait_per_byte_us);
    if (readable)
    {
        uart_read_blocking(uart, dst, len);
    }
    return readable;
}

// data is optional, can be NULL if length is 0
void rfid_7941w_send(uart_inst_t *uart, uint8_t address, uint8_t command, uint8_t length, uint8_t *data)
{
    //Protocol Header | Address | Command | Data Length | Data        | XOR Check
    //AB BA           | 1 Byte  | 1 Byte  | 1 Byte      | 0-255 Bytes | 1 Byte
    //XOR check: result of other bytes check except protocol header.

    uint8_t buffer[261]; 

    buffer[0] = 0xAB;
    buffer[1] = 0xBA;  
    buffer[2] = address; 
    buffer[3] = command;  
    buffer[4] = length;

    if (length!=0 && data!=NULL) 
    {
        memcpy((void *)(buffer+5), (void *)data, length);
    }
    buffer[5+length] = calc_xor(length+3, (buffer+2) );

    uart_write_blocking(uart, buffer, (6+length));
}

// data must have enough space for 255 bytes, returns true on success
bool rfid_7941w_recv(uart_inst_t *uart, uint8_t *address, uint8_t *command, uint8_t *length, uint8_t *data)
{
    //Protocol Header | Address | Command | Data Length | Data        | XOR Check
    //CD DC           | 1 Byte  | 1 Byte  | 1 Byte      | 0-255 Bytes | 1 Byte
    //XOR check: result of other bytes check except protocol header.
    
    uint8_t buffer[261]; 

    if (! uart_read_blocking_timout(uart, buffer, 5) )
    {
        return false;
    }

    if (buffer[0]==0xCD && buffer[1]==0xDC)
    {
        uint8_t length_local = buffer[4];

        if (address)
        {
            *address = buffer[2];
        }
        if (command)
        {
            *command = buffer[3];
        }
        if (length)
        {
            *length = length_local;
        }

        if (! uart_read_blocking_timout(uart, (buffer+5), length_local+1) )
        {
            return false;
        }

        uint8_t xor = calc_xor(length_local+3, (buffer+2) );
        
        if (buffer[5+length_local] == xor)
        {
            if (length_local!=0 && data!=NULL) 
            {
                memcpy((void *)data, (void *)(buffer+5), length_local);
            }
            return true;
        }
    }

    return false;
}