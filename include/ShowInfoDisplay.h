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

enum class TypeShowDisplay
{
    startShow,
    youtubeSubscriber,
    sensorDHT11,
    openWeather,
    endShow
};

TypeShowDisplay& operator++(TypeShowDisplay& type)
{
    using IntType = typename std::underlying_type<TypeShowDisplay>::type;
    type = static_cast<TypeShowDisplay>(static_cast<IntType>(type) + 1);
    if (type == TypeShowDisplay::endShow)
    {
        type = TypeShowDisplay::startShow;
    }
    return type;
}

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

class ShowInfoDisplay
{
private:
    Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
public:
    bool flagDot = false;
    TypeShowDisplay flagShowData = TypeShowDisplay::startShow;
    ShowInfoDisplay();
    void Init();
    void Welcome();
    void ShowTimeDate();
    void ShowYouTubeInfo(uint32_t subscriber, uint32_t view, uint32_t video);
    void ShowTempAndHum(float temp, int hum);
    void ShowOpenWeather(float temp, int hum, int pressure, float speed);
    ~ShowInfoDisplay();
};

ShowInfoDisplay::ShowInfoDisplay()
{
    //TODO:
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
    uint8_t madctl = ST77XX_MADCTL_MX | ST77XX_MADCTL_MY | ST77XX_MADCTL_RGB;
    tft.sendCommand(ST77XX_MADCTL, &madctl, 1);
    tft.fillScreen(TFT_BACKGRAUND_COLOR);
}

inline void ShowInfoDisplay::Welcome()
{
    int16_t x, y;
    uint16_t w, h;

    String bufStr;

    bufStr = String("IoT Smart Clock " + String(BOARD_FIRMWARE_VERSION));

    tft.setTextSize(1U);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(ST7735_ORANGE, TFT_BACKGRAUND_COLOR);
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), 0);
    tft.print(bufStr);
}

inline void ShowInfoDisplay::ShowTimeDate()
{
    int16_t x, y;
    uint16_t w, h;

    struct tm timeinfo;

    getLocalTime( &timeinfo );

    String bufStr;
    
    char bufChar[6];
    
    if(flagDot)
    {
        sprintf(bufChar, "%02d:%02d", timeinfo.tm_hour + TIME_ZONE, timeinfo.tm_min);
    }    
    else
    {
        sprintf(bufChar, "%02d %02d", timeinfo.tm_hour + TIME_ZONE, timeinfo.tm_min);
    }
    
    bufStr = String(bufChar);

    tft.setTextSize(TFT_CLOCK_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(TFT_CLOCK_COLOR, TFT_BACKGRAUND_COLOR);
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), TFT_CLOCK_Y);
    tft.print(bufStr);

    sprintf(bufChar, "%02d", timeinfo.tm_mday);

    bufStr = "   " + String(bufChar) + " " + months[timeinfo.tm_mon] + " " + String(timeinfo.tm_year - 100U) + "   ";

    tft.setTextSize(TFT_DATE_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    tft.setTextColor(TFT_DATE_COLOR, TFT_BACKGRAUND_COLOR);
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), TFT_DATE_Y);
    tft.print(bufStr);
    
    bufStr = "  " + weekDays[timeinfo.tm_wday] + "  ";

    tft.setTextSize(TFT_WEEKDAY_SIZE);
    tft.getTextBounds(bufStr, 0, 0, &x, &y, &w, &h);
    
    if(timeinfo.tm_mday == 0 || timeinfo.tm_mday == 6)
    {
        tft.setTextColor(TFT_WEEKENDS_COLOR, TFT_BACKGRAUND_COLOR);
    }
    else
    {
        tft.setTextColor(TFT_WEEKDAYS_COLOR, TFT_BACKGRAUND_COLOR);
    }
    
    tft.setCursor((ST7735_TFTWIDTH_128 / 2) - (w / 2), TFT_WEEKDAY_Y);
    tft.print(bufStr);
}

inline void ShowInfoDisplay::ShowYouTubeInfo(uint32_t subscriber, uint32_t view, uint32_t video)
{
    // const int json_memory_buffer = JSON_MEMORY_BUFFER;
    uint32_t youtube_subscriber_count = subscriber;
    uint32_t youtube_view_count = view;
    uint32_t youtube_video_count = video;

    char display_line[80];
    
    uint8_t starting_y_for_text = 80;
    uint8_t vertical_spacing = 4;
    uint8_t line_number = 0;
    int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
    uint16_t x, y;

    tft.fillRect(0, starting_y_for_text, ST7735_TFTWIDTH_128, ST7735_TFTHEIGHT_160 - starting_y_for_text, TFT_BACKGRAUND_COLOR);

    tft.setTextColor(ST7735_WHITE, TFT_BACKGRAUND_COLOR);
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

void ShowInfoDisplay::ShowTempAndHum(float temp, int hum)
{
    uint8_t starting_y_for_text = 80;
    uint8_t vertical_spacing = 4;
    uint8_t line_number = 0;
    char display_line[80];
    
    int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
    uint16_t x, y;

    tft.fillRect(0, starting_y_for_text, ST7735_TFTWIDTH_128, ST7735_TFTHEIGHT_160 - starting_y_for_text, TFT_BACKGRAUND_COLOR);

    tft.setTextColor(ST7735_YELLOW, TFT_BACKGRAUND_COLOR);

    sprintf(display_line, "Temperature:");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text;
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%.1f", temp);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    sprintf(display_line, "Humidity:");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", hum);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;
}

void ShowInfoDisplay::ShowOpenWeather(float temp, int hum, int pressure, float speed)
{
    uint8_t starting_y_for_text = 80;
    uint8_t vertical_spacing = 4;
    uint8_t line_number = 0;
    char display_line[80];
    
    int16_t tbx, tby; uint16_t tbw, tbh; // boundary box window
    uint16_t x, y;

    tft.fillRect(0, starting_y_for_text, ST7735_TFTWIDTH_128, ST7735_TFTHEIGHT_160 - starting_y_for_text, TFT_BACKGRAUND_COLOR);

    tft.setTextColor(ST7735_GREEN, TFT_BACKGRAUND_COLOR);

    sprintf(display_line, "Temperature:");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text;
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%.1f", temp);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    sprintf(display_line, "Humidity:");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", hum);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;

    sprintf(display_line, "Pressure:");
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    ++line_number;

    sprintf(display_line, "%d", pressure);
    tft.getTextBounds(display_line, 0, 0, &tbx, &tby, &tbw, &tbh); // it works for origin 0, 0, fortunately (negative tby!)
    x = ((tft.width() - tbw) / 2) - tbx;
    y = starting_y_for_text+((tbh+vertical_spacing)*line_number);    // tbh height of font + a few pixels
    tft.setCursor(x, y); // set the postition to start printing text
    tft.print(display_line); // print some text
    line_number += 1;
}

ShowInfoDisplay::~ShowInfoDisplay()
{
    //TODO:
}

ShowInfoDisplay tftDisplay;
