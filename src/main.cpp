#include <Arduino.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS        17 // PyBadge/PyGamer display control pins: chip select
#define TFT_RST       27 // Display reset
#define TFT_DC        16 // Display data/command select
#define TFT_BACKLIGHT 25 // Display backlight pin
#define TFT_MOSI      14  // Data out
#define TFT_SCLK      12  // Clock out

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) 
{
  Serial.begin(115200);
  Serial.print(F("Hello! ESP32 IoT Smart Watch!"));

  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on

  tft.initR(INITR_GREENTAB);        // Initialize ST7735R screen
  tft.setRotation(0);

  Serial.println(F("Initialized"));

  tft.fillScreen(ST77XX_BLACK);
}

void loop() 
{
  
}
