#include "Arduino.h"


int pin = 13;
uint64_t pretime = 0;

IRAM_ATTR void pinEventChange() {
  int value = digitalRead(pin);
  uint64_t time = micros64();
  uint64_t delta = time - pretime;
  pretime = time;

  Serial.println(value);

}





void setup() 
{
  Serial.begin(115200);
  pinMode(pin, INPUT);
  attachInterrupt(pin, pinEventChange, CHANGE);

}



void loop() 
{
  uint64_t time = micros64();
  uint64_t delta = time - pretime;
  if (delta > 1000000) {
    pretime = time;
    int value = digitalRead(pin);
    Serial.println(value);
  }
}
