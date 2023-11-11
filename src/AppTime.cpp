#include <Arduino.h>

#include "def.h"
#include "AppTime.h"

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -25200;
const int   daylightOffset_sec = 0;

AppTime::AppTime() {}

AppTime::~AppTime() {}

void AppTime::config() {
    configTime(0, 0, NTP_SERVER);
    setenv("TZ", TIMEZONE, 1);
    tzset();

    struct tm timeinfo;
    getLocalTime(&timeinfo);
}

void AppTime::printLocalTime() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    Serial.print(timeinfo.tm_hour);
    Serial.print(":");
    Serial.print(timeinfo.tm_min);
    Serial.print(":");
    Serial.println(timeinfo.tm_sec);
}

bool AppTime::isTimeToSleep() {
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return true;
    }
    
    return timeinfo.tm_hour >= OFF_HOUR || timeinfo.tm_hour < ON_HOUR;
}

bool AppTime::isTimeBeforeSleep() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
    }

    int hourTarget = OFF_HOUR;
    
    return timeinfo.tm_hour == hourTarget - 1 && timeinfo.tm_min >= 60 - OFF_NOTIFICATION_MINUTES;
}

unsigned long AppTime::getDelay() {
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        unsigned long oneMinuteDelay = 1000 * 60;
        return oneMinuteDelay;
    }

    int hourTarget = OFF_HOUR;
    bool lastHour = timeinfo.tm_hour == hourTarget - 1;
    unsigned long minutes = (60 - timeinfo.tm_min) * 60 * 1000;
    unsigned long seconds = (60 - timeinfo.tm_sec) * 1000;

    if (lastHour) {
        return seconds + 1000;
    }

    return minutes + seconds + 1000;
}

