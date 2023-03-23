#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/uart.h"

#define UART_ID uart1
#define UART_TX_PIN 4
#define UART_RX_PIN 5
// 9600–8-N-1
#define BAUD_RATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE


void send_data()
{
    const uint8_t buffer[] = {0xAB, 0xBA, 0x00, 0x15, 0x00, 0x15 };
    uart_write_blocking(UART_ID, buffer, sizeof(buffer));
    printf("Data sent\n");
}

void recv_data()
{
    uint8_t buffer[5];
    uart_read_blocking(UART_ID, buffer, sizeof(buffer));
    printf("%02hhX\n",buffer[0]);
}

int main() {
    stdio_init_all();

    uart_init(UART_ID, 2400);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    int __unused actual = uart_set_baudrate(UART_ID, BAUD_RATE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    send_data();
    recv_data();
    

    const uint LED_PIN = 28;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    int counter = 0;
    while (true) {
        printf("Hello, world! %d\n", counter++);
        gpio_put(LED_PIN, counter % 2);
        sleep_ms(1000);
    }
    return 0;
}
