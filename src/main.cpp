#include "def.h"
#include "AppWiFi.h"
#include "AppTime.h"
#include "Led.h"
#include "Relay.h"

void setup() {
    Serial.begin(SERIAL_BAUD);

    Relay::init();
    Relay::off();

    Led::init();

    AppWiFi::connect();
    AppTime::config();
    AppWiFi::disconnect();
}

void loop() {
    AppTime::printLocalTime();

    bool isOn = Relay::isOn();

    bool isTimeToSleep = AppTime::isTimeToSleep();
    
    if (isTimeToSleep && isOn) {
        Relay::off();
    } else if (!isTimeToSleep && !isOn) {
        Relay::on();
    }

    Led::control();
    
    delay(AppTime::getDelay());
}