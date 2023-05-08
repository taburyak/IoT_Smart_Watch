#pragma once

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "SettingsIoT.h"

#define TFT_CS                  5U   // PyBadge/PyGamer display control pins: chip select
#define TFT_RST                 13U  // Display reset
#define TFT_DC                  12U  // Display data/command select
#define TFT_BACKLIGHT           14U  // Display backlight pin
#define TFT_MOSI                23U  // Data out
#define TFT_SCLK                18U  // Clock out

class TFTDisplay
{
private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
public:
    TFTDisplay();
    void TFTDisplayWelcome();
    ~TFTDisplay();
};

TFTDisplay::TFTDisplay()
{
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

    tft.initR(INITR_GREENTAB);
    tft.setRotation(0);
    tft.fillScreen(ST77XX_BLACK);
}

void TFTDisplay::TFTDisplayWelcome()
{
    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(1);
    tft.println("Welcome!");
}

TFTDisplay::~TFTDisplay()
{
}

TFTDisplay tftDisplay;
