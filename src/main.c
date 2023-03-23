#include <stdio.h>
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

    uint8_t len;
    uint8_t buff[255];
    
    //uint8_t w[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    //rfid_7941w_write_LF(uart1, sizeof(w), w);

    //uint8_t w[] = {0x11, 0x22, 0x33, 0x44};
    //rfid_7941w_write_HF(uart1, sizeof(w), w);

    //sleep_ms(3000);

    int counter = 0;
    while (true) {
        printf("Loop %d\n", counter++);
        

        if (rfid_7941w_read(uart1, &len, buff))
        {
            printf("  Received id: 0x");
            int i = 0;
            for (i=0;i<len;i++) 
            {
                printf("%02hhX", buff[i]);
            }
            printf(" (%010u)", extract_uint32(len, buff));
            printf("\n");
        }

        sleep_ms(950);
    }
    return 0;
}
