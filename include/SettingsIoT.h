#pragma once

#define BOARD_FIRMWARE_VERSION  "0.0.1"
#define BOARD_HARDWARE_VERSION  "0.0.1"

#define TFT_CS                  5U  // PyBadge/PyGamer display control pins: chip select
#define TFT_RST                 13U  // Display reset
#define TFT_DC                  12U  // Display data/command select
#define TFT_BACKLIGHT           14U  // Display backlight pin
#define TFT_MOSI                23U  // Data out
#define TFT_SCLK                18U  // Clock out

typedef struct
{
    bool flagConfig;
}ConfigStore_t;

ConfigStore_t configStore = {.flagConfig = false};
