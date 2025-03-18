#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    Serial.println();

}

void loop()
{
    Serial.write(0);
    delay(1000);
    Serial.write(1);
    delay(1000);
    Serial.write(2);
    delay(1000);
}
