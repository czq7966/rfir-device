#include "dooya-DVQ24CF.h"


void rfir::module::device::curtain::Dooya_DVQ24CF::start(void * p) {
    Dooya::start(p);
    d485 = new Dooya485(Serial, 5, 5);    
}

void rfir::module::device::curtain::Dooya_DVQ24CF::loop() {
    Dooya::loop();

    return;


    static unsigned long temp_time = millis();
    static unsigned long read_mode = 0;
    neb::CJsonObject pld;
    if (millis() - temp_time >= 3 * 1000) {
        switch (read_mode)
        {
            case 0:                                
                pld.ReplaceAdd("code", "0x55 FE FE 03 01 B9 24");
                if (onCmd_set(&pld))
                // if (d485->open())                
                    DEBUGER.println("打开成功");
                else 
                    DEBUGER.println("打开失败");

                read_mode = 1;
                break;
            case 1:
                pld.ReplaceAdd("code", "0x55 FE FE 03 03 38 E5");
                if (onCmd_set(&pld))
                // if (d485->stop())
                    DEBUGER.println("停止成功");
                else 
                    DEBUGER.println("停止失败");

                read_mode = 2;
                break;
            case 2:
                pld.ReplaceAdd("code", "0x55 FE FE 03 02 F9 25");
                if (onCmd_set(&pld))
                // if (d485->close())
                    DEBUGER.println("关闭成功");
                else 
                    DEBUGER.println("关闭失败");

                read_mode = 3;
                break;                                
            
            default:
                read_mode = 0;
                break;
        }

        // if (!read_mode) {
        //     if (d485->open())
        //         DEBUGER.println("打开成功");
        //     // d485->readReg(0x00, 0x01);
        //     read_mode = true;
        // } else {
        //     if (d485->stop())
        //         DEBUGER.println("停止成功");
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
    std::string code;
    pld->Get("code", code);
    auto nbytes = rfir::util::Util::StringToBytes(code, d485->txBuf) / 8;
    
    if (nbytes > 0) {
        uint8_t len = 0;
        return d485->sendCodeAndRecv(d485->txBuf, nbytes, d485->rxBuf, len);
    }
    
    return 0;
}

bool rfir::module::device::curtain::Dooya_DVQ24CF::onCmd_get(neb::CJsonObject* pld) {
    std::string code = "0x55 FE FE 01 00 10 44 2E";

    auto nbytes = rfir::util::Util::StringToBytes(code, d485->txBuf) / 8;
    if (nbytes > 0) {
        uint8_t len = 0;
        if(d485->sendCodeAndRecv(d485->txBuf, nbytes, d485->rxBuf, len)) {
            code = rfir::util::Util::BytesToHexString(d485->rxBuf, len);
            pld->Add("code", code);
        } else {            
            code = rfir::util::Util::BytesToHexString(d485->rxBuf, len);
            pld->Add("errCode", code);
        } 
    }
    
    return 0;

}