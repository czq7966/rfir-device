#include "cl-vrhall-ff-okonoff.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_FF_Okonoff::init() {
    auto d = Okonoff::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 14;
    d->packet.sniff.params.bufSize = 512;
    //发码参数 
    d->packet.send.params.pin = 4;
    d->packet.send.params.repeat = 1;
#else     
    //采码参数
    d->packet.sniff.params.pin = 15;
    // 发码参数 
    d->packet.send.params.pin = 22;  
#endif    

    return d;
}


void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::start(void * p) {
    Okonoff::start(p);

}


void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::loop() {
    Okonoff::loop();

    // static auto CL_VRHALL_FF_Okonoff_test_time = millis();
    // std::string code = "3491,1671,566,1153,568,337,569,1155,567,341,569,336,570,1157,565,1154,568,341,569,336,570,1155,566,337,569,1160,565,1155,567,337,570,336,570,1158,568,1153,568,1154,567,1155,567,341,569,336,570,337,569,336,570,1160,565,338,569,336,569,337,570,1158,567,1154,568,1153,568,1156,566,343,567,338,569,336,570,337,569,342,568,338,568,338,569,337,569,342,569,336,570,337,569,336,570,1160,566,1156,565,337,570,336,570,342,568,1155,567,1154,567,1155,568,341,568,336,570,1155,566,1155,567,1159,566,1156,566,1155,566,337,569,1159,567,336,570,336,571,1154,567,1158,567,337,569,337,569,1156,566,342,569,1155,565,1157,566,337,569,342,568,337,569,337,569,337,570,342,568,336,571,336,569,337,569,343,568,1154,569,1153,566,1156,567,1157,567,1156,565,1157,565,1156,566,1159,567,337,569,337,569,336,569,344,567,338,569,336,570,337,568,343,568,1156,565,1156,565,1156,566,1159,567,1155,566,1155,567,1155,566,1159,567,336,572,336,568,336,570,343,567,337,570,335,571,336,569,336,569,65535,";
    // auto rfir = rfir::GetRfir();
    // if (millis() - CL_VRHALL_FF_Okonoff_test_time > 1000 * 2) {
    //     rfir->sender->sendRaw(code.c_str(), code.length());
    //     CL_VRHALL_FF_Okonoff_test_time = millis();
    //     Serial.println( CL_VRHALL_FF_Okonoff_test_time);        
    // }


}