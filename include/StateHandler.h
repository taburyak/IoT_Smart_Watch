#pragma once

#include <Arduino.h>
#include "SecretDefinitions.h"
#include "ShowInfoDisplay.h"
#include "SettingsIoT.h"
#include "WiFiConnect.h"
#include "TempAndHum.h"
#include <SimpleTimer.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char *apiKey      = YOUTUBE_API_KEY;
const char *channelId   = YOUTUBE_CHANNEL_ID;

SimpleTimer timer;

void timerOnceSecond();
void timerRefreshData();
void displayYouTubeSubscriberCount();

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
    if(tftDisplay.flagShowData)
    {
        displayYouTubeSubscriberCount();
    }
    else
    {
        tftDisplay.ShowTempAndHum(dht11.GetTemp(), dht11.GetHum());
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
    tftDisplay.flagShowData = !tftDisplay.flagShowData;
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
                    youtube_subscriber_count = -1;
                    youtube_view_count = -1;
                    youtube_video_count = -1;
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
