#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  uint8_t ss;     // Slave Select state
  uint8_t sck;    // Clock state
  uint8_t mosi;   // Master Out Slave In
  uint8_t miso;   // Master In Slave Out
  uint8_t irq;    // Interrupt signal
} chip_state_t;

void chip_init() {
  chip_state_t *chip = malloc(sizeof(chip_state_t));

  // Initialize chip state
  chip->ss = 1;    // Default: SS high (not selected)
  chip->sck = 0;   // Default: Clock low
  chip->mosi = 0;  // Default: MOSI low
  chip->miso = 0;  // Default: MISO low
  chip->irq = 0;   // Default: IRQ low

  // Set initial pin states (new pin mappings)
  pin_mode(4, OUTPUT); // IRQ pin
  pin_write(4, chip->irq);

  printf("RFID chip initialized.\n");
}

void chip_loop() {
  // Read pin states (updated pin mappings)
  uint8_t ss = pin_read(19);  // SS pin (Slave Select)
  uint8_t sck = pin_read(2);  // SCK pin (Clock)
  uint8_t mosi = pin_read(15); // MOSI pin (Master Out Slave In)

  // Detect when SS is asserted low (chip selected)
  if (ss == 0) {
    printf("RFID chip selected (SS low).\n");

    // Generate an IRQ signal as an example
    pin_write(4, 1); // Set IRQ high
   // delay(100);      // Hold IRQ for 100ms
    pin_write(4, 0); // Set IRQ low

    // Simulate SPI communication
    if (sck == 1) {
      // Example: Read a bit from MOSI and write a bit to MISO
      printf("SCK detected: MOSI = %d\n", mosi);
      pin_write(12, mosi); // Write the same value back on MISO (Pin 12)
    }
  } else {
    printf("RFID chip deselected (SS high).\n");
  }

    //delay(10); // Small delay to avoid flooding the console
}
