#pragma once

#include <Arduino.h>
#include "SecretDefinitions.h"
#include "TFTDisplay.h"
#include "SettingsIoT.h"

// Replace with your network credentials
const char *ssid        = MY_SSID;
const char *password    = MY_PASSWORD;
const char *apiKey      = YOUTUBE_API_KEY;
const char *channelId   = YOUTUBE_CHANNEL_ID;

void enterWaitConfig()
{
    ServiceState::set(MODE_INITIAL_PERIPH);
}

void enterInitialPeriph()
{
    tftDisplay.Init();
    ServiceState::set(MODE_DISPLAY);
}

void enterConfigMode()
{
    //TODO:
}

void enterConnectNet()
{
    //TODO:
}

void enterConnectCloud()
{
    //TODO:
}

void enterRun()
{
    //TODO:
}

void enterDisplay()
{
    tftDisplay.Welcome();
    ServiceState::set(MODE_RUNNING);
}

void enterRefreshData()
{
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
    ServiceState::set(MODE_DISPLAY);
}

void timerRefreshData()
{
    ServiceState::set(MODE_REFRESH_DATA);
}
