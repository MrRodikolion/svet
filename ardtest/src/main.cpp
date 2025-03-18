#include <Arduino.h>

void setup()
{
    Serial3.begin(115200);
    Serial.begin(115200);
    Serial.println();

    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    if (Serial3.available()){
        Serial.println(Serial3.read());
        digitalWrite(LED_BUILTIN, HIGH);
    }
    else{
        digitalWrite(LED_BUILTIN, LOW);
    }
}
