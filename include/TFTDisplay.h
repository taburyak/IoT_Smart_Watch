#pragma once

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "SettingsIoT.h"

#define TFT_CS                  GPIO_NUM_5   // PyBadge/PyGamer display control pins: chip select
#define TFT_RST                 GPIO_NUM_13  // Display reset
#define TFT_DC                  GPIO_NUM_12  // Display data/command select
#define TFT_BACKLIGHT           GPIO_NUM_14  // Display backlight pin
#define TFT_MOSI                GPIO_NUM_23  // Data out
#define TFT_SCLK                GPIO_NUM_18  // Clock out

class TFTDisplay
{
private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
public:
    TFTDisplay();
    void Init();
    void Welcome();
    ~TFTDisplay();
};

TFTDisplay::TFTDisplay()
{
    
}

inline void TFTDisplay::Init()
{
    gpio_config_t blPin;
    blPin.mode = GPIO_MODE_OUTPUT;
    blPin.pin_bit_mask = (1ULL << TFT_BACKLIGHT);
    gpio_config(&blPin);
    gpio_set_level(TFT_BACKLIGHT, HIGH);

    tft.initR(INITR_GREENTAB);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_BLACK);
}

inline void TFTDisplay::Welcome()
{
    int16_t x, y;
    uint16_t w, h;

    String bufStr;

    bufStr = String("IoT Smart Clock " + String(BOARD_FIRMWARE_VERSION));

    tft.setTextSize(1U);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), (ST7735_TFTHEIGHT_160 / 2) - (h / 2));
    tft.print(bufStr);
}

TFTDisplay::~TFTDisplay()
{
}

TFTDisplay tftDisplay;
