
// #include <Arduino.h>
// #include <IRremoteESP8266.h>
// #include <IRsend.h>
// #include <IotWebConf.h>

// const uint16_t kIrLed = 5;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

// IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.

// // Example of data captured by IRrecvDumpV2.ino
// uint16_t rawData[134] = {4600,2600,360,400,360,950,360,950,360,400,360,950,360,400,360,400,360,400,360,400,360,950,360,400,360,400,360,950,360,400,360,400,360,400,360,950,360,950,360,950,360,400,360,950,360,400,360,950,360,400,360,950,360,950,360,400,360,400,360,400,360,950,360,400,360,400,360,400,360,400,360,400,360,400,360,950,360,400,360,400,360,400,360,400,360,400,360,400,360,400,360,950,360,400,360,400,360,400,360,950,360,950,360,400,360,400,360,400,360,950,360,400,360,400,360,400,360,400,360,400,360,950,360,400,360,950,360,400,360,950,360,20500,4600,20000};

// uint8_t state[kMcQuayStateLength] = {0x16 , 0x12 , 0x18 , 0x04 , 0x13 , 0x10 , 0x16 , 0x30};
// // uint8_t state[kMcQuayStateLength] = {0x16 , 0x12 , 0x18 , 0x04 , 0x13 , 0x10 , 0x17 , 0x40};
// // uint8_t state[kMcQuayStateLength] = {0x16 , 0x12 , 0x18 , 0x04 , 0x13 , 0x10 , 0x18 , 0x50};


// void setup() {
//   irsend.begin();
// #if ESP8266
//   GDebuger.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
// #else  // ESP8266
//   GDebuger.begin(115200, SERIAL_8N1);
// #endif  // ESP8266
// }

// void loop() {
//   GDebuger.println("a McQuay A/C state ");
//   irsend.sendMcQuay(state);
//   delay(2000);
// }
