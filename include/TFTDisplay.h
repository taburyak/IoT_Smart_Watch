#pragma once

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>
#include "SettingsIoT.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void tftInit(void)
{
    pinMode(TFT_BACKLIGHT, OUTPUT);
    digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

    tft.initR(INITR_GREENTAB);
    tft.setRotation(0);

    tft.fillScreen(ST77XX_BLACK);

    tft.setCursor(0, 0);
    tft.setTextColor(ST77XX_YELLOW);
    tft.setTextSize(1);
    tft.println("Starting!");
}