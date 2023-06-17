# IoT_Smart_Watch

# Project Description

This project is based on the ESP8266 or ESP32 WiFi module, TFT display, temperature and humidity sensor, such as DHT11(22), BUZZER (optional), RTC module (optional).

Functionality:
- Display current time, calendar (synchronized with a time server), alarm (optional)
- Display temperature and humidity from the DHT sensor, local weather forecast
- Display the number of subscribers, views, and videos from your YouTube channel (optional)
- Custom web server on the microcontroller for configuration settings
- Over-the-air firmware update (optional)

Mandatory features:
- Device can be configured via WiFi by connecting to its access point
- Device can connect to any WiFi network for internet access
- TFT display shows:
    - Time, calendar (date, month, year, day of the week) taken from a time service on the internet
    - Temperature and humidity from the DHT sensor, weather forecast from a weather service
    - The number of subscribers, views, and videos on your YouTube channel (enabled in settings)

Optional features:
- RTC module
- Alarm clock
- Over-the-air firmware update
- Temperature and humidity readings are sent to a server such as https://thingspeak.com/ (See example: https://thingspeak.com/channels/2193007)
