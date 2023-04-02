# Raspberry Pi 7941W RFID/Keyfob Reader/Writer

## Pinout
Connect the following
 - 7941W 5V / Level shifter HV (AVCC) / Pico VSYS (5V)
 - 7941W RX / Level shifter HV1 (ASCL)
 - 7941W TX / Level shifter HV2 (ASDA)
 - 7941W GND / Level shifter GND L/H (A/B) / Pico GND
 - 7941W IO Not connected
 - Level shifter LV (BVCC) / Pico 3v3
 - Level shifter LV1 (BSCL) / Pico GP4 (Uart1 TX)
 - Level shifter LV2 (BSDA) / Pico GP5 (Uart1 RX)

## Adding the library
```
add_subdirectory(./lib/7941w)
target_link_libraries(... 7941w)
```
 
