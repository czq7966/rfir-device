#include "dooya-DVQ24CF.h"


void rfir::module::device::curtain::Dooya_DVQ24CF::start(void * p) {
    Dooya::start(p);
    d485 = new Dooya485(Serial, 5, 5);    
}

void rfir::module::device::curtain::Dooya_DVQ24CF::loop() {
    Dooya::loop();
    d485->init();

    return;


    static unsigned long temp_time = millis();
    static unsigned long read_mode = 0;
    if (millis() - temp_time >= 3 * 1000) {
        switch (read_mode)
        {
            case 0:
                if (d485->open())
                    Serial.println("打开成功");
                else 
                    Serial.println("打开失败");

                read_mode = 1;
                break;
            case 1:
                if (d485->stop())
                    Serial.println("停止成功");
                else 
                    Serial.println("停止失败");

                read_mode = 2;
                break;
            case 2:
                if (d485->close())
                    Serial.println("关闭成功");
                else 
                    Serial.println("关闭失败");

                read_mode = 3;
                break;                                
            
            default:
                read_mode = 0;
                break;
        }

        // if (!read_mode) {
        //     if (d485->open())
        //         Serial.println("打开成功");
        //     // d485->readReg(0x00, 0x01);
        //     read_mode = true;
        // } else {
        //     if (d485->stop())
        //         Serial.println("停止成功");
        //     // d485->readReg(0x00, 0x02);
        //     read_mode = false;
        // }

        temp_time = millis();
    }

    // d485->startRecvData(d485->rxBuf, 7, 100);

}

rfir::module::ttl::Config::Device* rfir::module::device::curtain::Dooya_DVQ24CF::init() {
    auto d = Dooya::init();
    return d;
}

bool rfir::module::device::curtain::Dooya_DVQ24CF::onCmd_set(neb::CJsonObject* pld) {
    return 0;

}

bool rfir::module::device::curtain::Dooya_DVQ24CF::onCmd_get(neb::CJsonObject* pld) {
    return 0;
}