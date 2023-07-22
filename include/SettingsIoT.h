#pragma once

#define BOARD_FIRMWARE_VERSION  "0.2.4"
#define BOARD_HARDWARE_VERSION  "0.0.2"

#define TIME_ZONE               (0U)

#define TFT_BACKGRAUND_COLOR    (ST7735_BLACK)
#define TFT_CLOCK_COLOR         (ST7735_YELLOW)
#define TFT_CLOCK_Y             (15U)
#define TFT_CLOCK_SIZE          (2U)
#define TFT_DATE_COLOR          (ST7735_GREEN)
#define TFT_DATE_Y              (TFT_CLOCK_Y + 27U)
#define TFT_DATE_SIZE           (1U)
#define TFT_WEEKDAYS_COLOR      (ST77XX_MAGENTA)
#define TFT_WEEKENDS_COLOR      (ST7735_RED)
#define TFT_WEEKDAY_Y           (TFT_DATE_Y + 15U)
#define TFT_WEEKDAY_SIZE        (1U)

#if !( defined(ESP8266) ||  defined(ESP32) )
  #error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

// Use from 0 to 4. Higher number, more debugging messages and memory usage.
#define _WIFIMGR_LOGLEVEL_    1

// To not display stored SSIDs and PWDs on Config Portal, select false. Default is true
// Even the stored Credentials are not display, just leave them all blank to reconnect and reuse the stored Credentials 
//#define DISPLAY_STORED_CREDENTIALS_IN_CP        false

#define ESP_WIFIMANAGER_VERSION_MIN_TARGET      "ESP_WiFiManager v1.12.0"
#define ESP_WIFIMANAGER_VERSION_MIN             1012000

#define JSON_MEMORY_BUFFER      (1024*1) // now much memory to give to the decoding of the json response
