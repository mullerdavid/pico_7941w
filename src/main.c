#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#include "lib/7941w.h"

//extracting last 4 bytes
uint32_t extract_uint32(uint8_t len, uint8_t* data)
{
    uint8_t* tail = (data+len-1);
    uint32_t ret = 0;
    int i = 0;
    while (i<4 && data<=tail)
    {
        ret = ret + ( ((uint32_t)(*tail)) << (i*8) );
        i++;
        tail--;
    }
    return ret;
}

int main() {
    stdio_init_all();

    rfid_7941w_init(uart1);

    //uint8_t len;
    //uint8_t buff[255];
    
    //uint8_t w[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    //rfid_7941w_write_LF(uart1, sizeof(w), w);

    //uint8_t w[] = {0x11, 0x22, 0x33, 0x44};
    //rfid_7941w_write_HF(uart1, sizeof(w), w);

    //sleep_ms(3000);

    int counter = 0;
    while (true) {
        printf("Loop %d\n", counter++);
        
        uint64_t id = rfid_7941w_alt_read(uart1);

        if (id != 0)
        {
            printf("  Received id: 0x%010" PRIx64 " (%010" PRIu32 ")\n", id, (uint32_t)id);
        }

        sleep_ms(950);
    }
    return 0;
}
