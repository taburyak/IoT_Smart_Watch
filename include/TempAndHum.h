#pragma once

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN     4     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11 // DHT 11

DHT_Unified dht(DHTPIN, DHTTYPE);

class TempAndHum
{
private:
    
public:
    TempAndHum();
    void InitSensor();
    float GetTemp();
    float GetHum();
    ~TempAndHum();
};

TempAndHum::TempAndHum()
{
    
}

void TempAndHum::InitSensor()
{
    dht.begin();
}

float TempAndHum::GetTemp()
{
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature))
    {
        Serial.println(F("Error reading temperature!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(event.temperature);
        Serial.println(F("°C"));
    }
    return event.temperature;
}

float TempAndHum::GetHum()
{
    sensors_event_t event;
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) 
    {
        Serial.println(F("Error reading humidity!"));
    }
    else
    {
        Serial.print(F("Humidity: "));
        Serial.print(event.relative_humidity);
        Serial.println(F("%"));
    }
    return event.relative_humidity;
}

TempAndHum::~TempAndHum()
{
}

TempAndHum dht11;
