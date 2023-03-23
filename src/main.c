#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "lib/7941w.h"

int main() {
    stdio_init_all();

    rfid_7941w_init(uart1);

    int counter = 0;
    while (true) {
        printf("Loop %d\n", counter++);
        
        rfid_7941w_send(uart1, 0x0, 0x10, 0, NULL);
        printf("  Data sent\n");
        sleep_ms(10);
        uint8_t command = 0;
        rfid_7941w_recv(uart1, NULL, &command, NULL, NULL);
        printf("  Received status %02hhX\n", command);

        sleep_ms(990);
    }
    return 0;
}
