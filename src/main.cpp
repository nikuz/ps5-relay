#include "def.h"
#include "AppWiFi.h"
#include "AppTime.h"
#include "Led.h"
#include "Relay.h"

unsigned long delayInterval = 1000 * 60; // one minute

void setup() {
    Serial.begin(SERIAL_BAUD);

    Relay::init();
    Relay::off();

    Led::init();

    AppWiFi::connect();
    AppTime::obtainInternetTime();
    AppWiFi::disconnect();
}

void loop() {
    bool isOn = Relay::isOn();

    bool isTimeToSleep = AppTime::isTimeToSleep();
    
    if (isTimeToSleep && isOn) {
        Relay::off();
    } else if (!isTimeToSleep && !isOn) {
        Relay::on();
    }

    Led::control();
    
    delay(delayInterval);
}