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

class ShowInfoDisplay
{
private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
public:
    ShowInfoDisplay();
    void Init();
    void Welcome();
    void ShowYouTubeInfo(int subscriber, int view, int video);
    ~ShowInfoDisplay();
};

ShowInfoDisplay::ShowInfoDisplay()
{
    
}

inline void ShowInfoDisplay::Init()
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

inline void ShowInfoDisplay::Welcome()
{
    int16_t x, y;
    uint16_t w, h;

    String bufStr;

    bufStr = String("IoT Smart Clock " + String(BOARD_FIRMWARE_VERSION));

    tft.setTextSize(1U);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(ST7735_GREEN, ST7735_BLACK);
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), 0);
    tft.print(bufStr);
}

inline void ShowInfoDisplay::ShowYouTubeInfo(int subscriber, int view, int video)
{
    // const int json_memory_buffer = JSON_MEMORY_BUFFER;
    int youtube_subscriber_count = subscriber;
    int youtube_view_count = view;
    int youtube_video_count = video;

    char display_line[80];
    
    int starting_y_for_text = 80;
    int vertical_spacing = 4;
    int line_number = 0;
    int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
    uint16_t x, y;

    tft.setTextColor(ST7735_WHITE, ST7735_BLACK);
    tft.setTextSize(1);

    // subscribers
    sprintf(display_line, "Subscribers");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text;
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_subscriber_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    // view count
    sprintf(display_line, "View Count");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_view_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    // video count
    sprintf(display_line, "Video Count");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", youtube_video_count);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;
}

ShowInfoDisplay::~ShowInfoDisplay()
{
}

ShowInfoDisplay tftDisplay;
