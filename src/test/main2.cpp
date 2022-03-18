// #include <Arduino.h>
// #include <Ticker.h>

// #include "rfir/service/serial/receiver.h"
// #include "rfir/service/serial/sender.h"
// #include "rfir/service/ttl/sniffer.h"
// #include "rfir/service/ttl/decoder.h"
// #include "rfir/service/ttl/sender.h"
// #include "rfir/module/cmds/cmd.h"

// #include "IRsend.h"
// #include "ir_Gree.h"

// const uint16_t kIrLed = 5;  // ESP8266 GPIO pin to use. Recommended: 4 (D2).

// IRsend irsend(kIrLed);  // Set the GPIO to be used to sending the message.
// //IRGreeAC irgree(kIrLed);

// // Example of data captured by IRrecvDumpV2.ino
// //uint16_t rawData[68] = {8799,4519,585,545,586,547,581,553,579,555,578,1651,578,561,571,559,573,560,573,562,570,562,566,568,570,564,566,1663,571,566,566,1664,567,567,565,571,562,571,562,1665,566,576,557,571,562,576,556,1675,554,577,558,1679,555,1674,557,584,554,1674,557,1675,559,1673,558,576,554,1675,551,18179};
// //uint16_t rawData[68] = {8516,4517,586,545,582,551,585,550,578,556,579,1650,579,556,576,560,569,562,569,562,574,562,569,562,570,566,568,1666,566,570,561,1668,566,568,561,568,567,569,567,1668,560,1671,561,574,558,578,558,1675,555,1674,561,1670,563,1673,561,574,552,580,556,1681,552,1673,557,580,550,580,550,18179};
//                       //9256,4492,622,514,627,511,616,530,605,539,595,1639,595,548,591,549,596,548,589,556,588,553,590,555,584,558,588,1649,584,568,580,1655,584,558,587,560,583,560,580,1654,581,574,565,569,574,575,568,1670,561,573,572,1676,560,1675,567,582,560,1669,568,1674,573,1667,573,568,561,1676,553,65535
//                       //8940,4541,673,560,676,554,662,576,655,577,652,1686,651,585,653,581,643,589,645,589,639,594,632,600,634,601,631,1705,637,600,707,1709,629,633,630,608,626,605,624,2284,625,615,615,1032,618,1383,621,2478,618,613,621,2144,617,1719,618,624,614,2247,624,1717,627,1716,622,614,615,1720,616,65535
// // Example Samsung A/C state captured from IRrecvDumpV2.ino
// // uint8_t samsungState[kSamsungAcStateLength] = {
// //     0x02, 0x92, 0x0F, 0x00, 0x00, 0x00, 0xF0,
// //     0x01, 0xE2, 0xFE, 0x71, 0x40, 0x11, 0xF0};

// uint8_t ytStateLeft[4] = {0x08, 0x0a, 0x11, 0xee};
// uint8_t ytStateRight[4] = {0x08, 0x0a, 0x22, 0xdd};
// uint8_t ytStateUp[4] = {0x08, 0x0a, 0x33, 0xcc};
// uint8_t ytStateDown[4] = {0x08, 0x0a, 0x44, 0xbb};

// //uint16_t rawData[68] = {9000,4500,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,1600,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,1600,620,1600,620,1600,620,1600,620,540,620,1600,620,1600,620,1600,620,540,600,18000};
// uint16_t rawOpen[140] = {9000,4500,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,1600,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,1600,620,540,620,540,620,1600,620,540,620,19000,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,1600,620,1600,620,19000};
// uint16_t rawClose[140] = {9000,4500,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,1600,620,540,620,540,620,1600,620,540,620,19000,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,540,620,1600,620,540,620,1600,620,540,620,19000};

// uint16_t rawData[263] = {653,638,645,653,646,645,647,661,630,646,645,653,638,653,645,669,2566,2597,622,661,653,1283,661,653,638,646,653,637,638,653,661,638,1291,1298,638,700,1253,1291,645,653,1291,1291,653,630,661,645,1291,646,645,1306,1283,653,646,645,646,1290,1291,661,646,1290,646,645,653,661,632,645,654,637,661,638,653,653,1283,653,638,661,645,638,653,1291,645,638,653,646,645,653,646,661,630,645,653,653,646,653,638,653,645,638,661,630,661,663,637,653,2582,2581,653,638,653,1299,653,637,661,646,645,646,653,638,653,637,1505,1086,653,638,1306,1291,645,654,1292,1299,645,638,653,637,1307,637,646,1291,1306,645,646,653,645,1283,1299,653,646,1282,654,653,653,638,653,638,661,637,653,677,614,663,1275,661,638,637,653,654,637,1291,653,653,646,638,653,645,653,646,645,653,646,653,638,653,645,646,653,638,653,645,646,653,645,638,2597,2582,661,637,653,1299,638,653,637,654,653,645,638,653,653,646,1298,1291,653,638,1298,1291,661,638,1298,1299,645,687,609,735,1206,645,653,1291,1299,645,645,654,645,1291,1295,646,653,1283,661,638,645,645,654,653,645,638,653,638,645,661,1283,654,653,653,638,647,630,1306,645,646,653,645,646,645,638};
// /*---------------------- 定义任务 ---------------------*/
// void Task1_blink(void *pvParameters)  // This is a task.
// {
//   while (true) {
//   rfir::service::serial::Receiver::dealCmd();
//   // rfir::service::ttl::Sniffer::sniff();
//   rfir::service::ttl::Decoder::decode();
//   rfir::service::serial::Sender::dealCmd();
//   delay(1);
//   }

// }
// void Task2_print(void *pvParameters){

// }
// void Task3_Print(void *pvParameters){

// }


// void setup() {
//   //irgree.begin();
//   irsend.begin();
//   DEBUGER.begin(115200, SERIAL_8N1, SERIAL_FULL);

//   // rfir::module::ttl::Sender::Params p1;
//   // p1.pin = kIrLed;   
//   // p1.modulation = false;
//   // p1.repeat = 0;
//   // rfir::service::ttl::Sender::init(p1);  

// }



// void loop() {
//   // DEBUGER.println("NEC");
//   // irsend.sendNEC(0x00FFE01FUL);
//   // delay(2000);
//   // DEBUGER.println("Sony");
//   // irsend.sendSony(0xa90, 12, 2);  // 12 bits & 2 repeats
//   // delay(2000);

//   // DEBUGER.println("a Samsung A/C state from IRrecvDumpV2");
//   // irsend.sendSamsungAC(samsungState);
//   // delay(2000);
//     // timeval t1,t2,t3;
//     // gettimeofday(&t1, NULL);
//     // timercmp(&t1, &t2, >);
//   // rfir::service::serial::Receiver::dealCmd();
//   // //rfir::service::ttl::Sniffer::sniff();
//   // rfir::service::ttl::Decoder::decode();
//   // rfir::service::serial::Sender::dealCmd();   
  
//   DEBUGER.println("a rawData capture from IRrecvDumpV2");
//   irsend.sendRaw(rawData, 263, 38);  // Send a raw data capture at 38kHz.
//   delay(3000);
//   //rfir::service::ttl::Sender::ttlsender->sendRaw(rawData, 68);
//   //delay(2000);
//   // rfir::service::ttl::Sender::ttlsender->sendGeneric(ytStateLeft, 4);
//   // delay(2000);
//   // rfir::service::ttl::Sender::ttlsender->sendGeneric(ytStateRight, 4);
//   // delay(2000);
//   // rfir::service::ttl::Sender::ttlsender->sendGeneric(ytStateUp, 4);
//   // delay(2000);
//   // rfir::service::ttl::Sender::ttlsender->sendGeneric(ytStateDown, 4);
//   // delay(2000);      

//   // uint64_t data = 0;
//   // irsend.sendGree(data, 64);
//   //delay(2000);      
//   //irsend.sendGree()
//   // irgree.on();
//   // irgree.setTemp(21);
//   // irgree.send();
//   // delay(2000);

//   // irgree.on();
//   // irgree.setTemp(25);
//   // irgree.send();
//   // delay(2000);


// }
