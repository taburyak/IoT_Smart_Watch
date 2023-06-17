#pragma once

#include <Arduino.h>
#include "SecretDefinitions.h"
#include "ShowInfoDisplay.h"
#include "SettingsIoT.h"
#include "WiFiConnect.h"
#include "TempAndHum.h"
#include <SimpleTimer.h>
#include <ArduinoJson.h>
#include "ThingSpeak.h"

// Replace with your network credentials
const char *apiKey              = YOUTUBE_API_KEY;
const char *channelId           = YOUTUBE_CHANNEL_ID;

unsigned long myChannelNumber   = SECRET_CH_ID;
const char * myWriteAPIKey      = SECRET_WRITE_APIKEY;

SimpleTimer timer;
WiFiClient client;

void timerOnceSecond();
void timerRefreshData();
void displayYouTubeSubscriberCount();
void displayOpenWeather();

void enterWaitConfig()
{
    ServiceState::set(MODE_INITIAL_PERIPH);
}

void enterInitialPeriph()
{
    tftDisplay.Init();
    tftDisplay.Welcome();

    dht11.InitSensor();

    ServiceState::set(MODE_CONFIGURING);
}

void enterConfigMode()
{
    timer.setInterval(1000U, timerOnceSecond);
    timer.setInterval(1000U * 60U * 1U, timerRefreshData);

    ServiceState::set(MODE_CONNECTING_NET);
}

void enterConnectNet()
{
    ConnectManager.InitialConnect();
    ServiceState::set(MODE_CONNECTING_CLOUD);
}

void enterConnectCloud()
{
    displayYouTubeSubscriberCount();
    ThingSpeak.begin(client);
    ServiceState::set(MODE_RUNNING);
}

void enterRun()
{
    timer.run();

    // Call the double reset detector loop method every so often,
    // so that it can recognise when the timeout expires.
    // You can also call drd.stop() when you wish to no longer
    // consider the next reset as a double reset.
    drd->loop();

    // put your main code here, to run repeatedly
    check_status();
}

void enterDisplay()
{
    tftDisplay.ShowTimeDate();
    ServiceState::set(MODE_RUNNING);
}

void enterRefreshData()
{
    if(tftDisplay.flagShowData == TypeShowDisplay::startShow)
    {
        ++tftDisplay.flagShowData;
    }

    if(tftDisplay.flagShowData == TypeShowDisplay::youtubeSubscriber)
    {
        displayYouTubeSubscriberCount();
    }
    else if(tftDisplay.flagShowData == TypeShowDisplay::sensorDHT11)
    {
        tftDisplay.ShowTempAndHum(dht11.GetTemp(), dht11.GetHum());
    }
    else if(tftDisplay.flagShowData == TypeShowDisplay::openWeather)
    {
        displayOpenWeather();
    }

    ThingSpeak.setField(1, dht11.GetTemp());
    ThingSpeak.setField(2, dht11.GetHum());

    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if(x == 200)
    {
        Serial.println("Channel update successful.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }

    ServiceState::set(MODE_RUNNING);
}

void enterOTA()
{
    //TODO:
}

void enterSwitchToSTA()
{

}

void enterResetConfig()
{

}

void enterNoWiFiMode()
{

}

void enterNoCloudMode()
{

}

void enterIdle()
{
    //TODO:
}

void enterError()
{
    //TODO:
}

void timerOnceSecond()
{
    tftDisplay.flagDot = !tftDisplay.flagDot;
    ServiceState::set(MODE_DISPLAY);
}

void timerRefreshData()
{
    ++tftDisplay.flagShowData;
    ServiceState::set(MODE_REFRESH_DATA);
}

//--------------------------------------------------------------------------------------------------
// displayYouTubeSubscriberCount()
//    Displays subscriber count, total number of public videos, and viewer count
//    using the globally defined youtube_channel_id and youtube_api_key vars
//--------------------------------------------------------------------------------------------------
void displayYouTubeSubscriberCount()
{
    // const int json_memory_buffer = JSON_MEMORY_BUFFER;
    uint32_t youtube_subscriber_count = 0;
    uint32_t youtube_view_count = 0;
    uint32_t youtube_video_count = 0;
  
    WiFiClientSecure client;
    client.setInsecure();      // as long as it is SSL we are good, not checking actual ssl key
    HTTPClient https;
    https.useHTTP10(true);    // required for deserializeJson (https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/)

    String restful_url = "https://www.googleapis.com/youtube/v3/channels?part=statistics&id=%youtube_channel_id%&key=%api_key%";

    String url = restful_url;
    url.replace("%youtube_channel_id%",channelId);
    url.replace("%api_key%",apiKey);

    Serial.println("displayYouTubeSubscriberCount()");
    Serial.print("calling url: ");
    Serial.println(url);
   
    if ( https.begin(client, url.c_str() )) 
    {  
        int httpCode = https.GET();
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        
        if (httpCode > 0) 
        {
            if (httpCode == 200 ) 
            {
                DynamicJsonDocument doc(JSON_MEMORY_BUFFER);
                DeserializationError error = deserializeJson(doc, https.getStream());
                if (error) 
                {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.f_str());
                    youtube_subscriber_count = 0;
                    youtube_view_count = 0;
                    youtube_video_count = 0;
                }
                else 
                {
                    Serial.println(https.getString());
                    youtube_subscriber_count = doc["items"][0]["statistics"]["subscriberCount"].as<int>();
                    youtube_view_count = doc["items"][0]["statistics"]["viewCount"].as<int>();
                    youtube_video_count = doc["items"][0]["statistics"]["videoCount"].as<int>();
                }
            }
        } 
    }

    https.end();

    tftDisplay.ShowYouTubeInfo(youtube_subscriber_count, youtube_view_count, youtube_video_count);
}

void displayOpenWeather()
{
    float temp = 0.0;
    int pressure = 0;
    float humidity = 0.0;
    float speed = 0.0;

    String openWeatherMapApiKey = OPEN_WEATHER_MAP_API_KEY;
    String city = CITY;
    String countryCode = COUNTRY_CODE;
    String jsonBuffer;

    WiFiClient client;
    // client.setInsecure();      // as long as it is SSL we are good, not checking actual ssl key
    HTTPClient https;
    https.useHTTP10(true);    // required for deserializeJson (https://arduinojson.org/v6/how-to/use-arduinojson-with-httpclient/)

    String restful_url = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;

    String url = restful_url;

    Serial.println("displayOpenWeather()");
    Serial.print("calling url: ");
    Serial.println(url);

    if ( https.begin(client, url.c_str() )) 
    {  
        int httpCode = https.GET();
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
        
        if (httpCode > 0) 
        {
            if (httpCode == 200 ) 
            {
                DynamicJsonDocument doc(JSON_MEMORY_BUFFER);
                DeserializationError error = deserializeJson(doc, https.getStream());
                if (error) 
                {
                    Serial.print("deserializeJson() failed: ");
                    Serial.println(error.f_str());
                    float temp = -1.0;
                    int pressure = -1;
                    float humidity = -1.0;
                    float speed = -1.0;
                }
                else 
                {
                    Serial.println(https.getString());
                    
                    const char* name = doc["name"];
                    Serial.print("City: ");
                    Serial.println(name);

                    temp = doc["main"]["temp"].as<float>() - 273.15;
                    Serial.print("Temperature: ");
                    Serial.println(temp);

                    humidity = doc["main"]["humidity"].as<float>();
                    Serial.print("Humidity: ");
                    Serial.println(humidity);
                    
                    pressure = doc["main"]["pressure"].as<int>();
                    Serial.print("Pressure: ");
                    Serial.println(pressure);

                    speed = doc["wind"]["speed"].as<float>();
                    Serial.print("Wind Speed: ");
                    Serial.println(speed);
                }
            }
        } 
    }

    https.end();

    tftDisplay.ShowOpenWeather(temp, humidity, pressure, speed);
}
