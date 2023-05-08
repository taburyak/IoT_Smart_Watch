#pragma once

#define BOARD_FIRMWARE_VERSION  "0.0.1"
#define BOARD_HARDWARE_VERSION  "0.0.1"

typedef struct
{
    bool flagConfig;
}ConfigStore_t;

ConfigStore_t configStore = {.flagConfig = false};
