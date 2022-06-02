#include "device.h"
#include "rfir/rfir.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/interrupt.h"


void rfir::module::device::Device::init() {
  
}

// void rfir::module::device::Device::emitChange(const char* reason) {
//     events.onChange.emit((void*)reason);
// }

void rfir::module::device::Device::start(void *) {
    loadConfig();   
}

void rfir::module::device::Device::loop() {
    // doTimerReport();
    // doRawChanged();
}

// bool rfir::module::device::Device::onCmd_set(neb::CJsonObject* pld) {
//     return 0;
// }

// bool rfir::module::device::Device::onCmd_get(neb::CJsonObject* pld) {
//     return 0;
// }

// bool rfir::module::device::Device::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
//     return 0;
// }

// bool rfir::module::device::Device::setRaw(uint8_t* raw) {
//     return 0;
// }

// uint8_t* rfir::module::device::Device::getRaw(int& count) {
//     return 0;
// }

// uint16_t* rfir::module::device::Device::getEncodeRaw(int& count) {
//     return 0;
// }

// void rfir::module::device::Device::onSetRaw() {
//     // saveRaw();
//     rawChanged = true;
//     rawChanged_time = millis();
// }



bool rfir::module::device::Device::loadConfig() {
    GInterrupt.stop();

    bool result = false;
    neb::CJsonObject json;
    std::string fn = "/"+ this->name + "-config.json";
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);    
    result = setConfig(context.c_str());

    GInterrupt.start();

    return result;



}

bool rfir::module::device::Device::saveConfig() {
    GInterrupt.stop();

    bool result = false;
    std::string context;    
    if (getConfig(context)) {
        neb::CJsonObject json;
        std::string fn = "/"+ this->name + "-raw.json";
        rfir::util::TxtFile file(fn.c_str());
        result =  file.writeString(context);
    }

    GInterrupt.start();
    
    return result;



    // std::string context;
    // file.readString(context);
    // if (context.length() > 0) {
    //     json.Parse(context);
    // }

    // json.ReplaceAdd("raw", "0x" + toHexString());
    // context = json.ToString();
    
}

bool rfir::module::device::Device::setConfig(const char* context){
    return false;
};

bool rfir::module::device::Device::getConfig(std::string& context){
    return false;
};  

// void rfir::module::device::Device::doTimerReport(bool reset) {
//     if (reset) {
//         reinitTimerReport(reset);
//     }


//     if (millis() - timerReport_LastTime > timerReport_Interval) {
//         this->events.onChange.emit((void*)"Timer Report");

//         if (timerReport_Interval == 1 * 1000) 
//             timerReport_Interval = 2 * 1000;
//         else 
//             timerReport_Interval = 60 * 1000;
        
//         timerReport_LastTime = millis();
//     }
// }

// void rfir::module::device::Device::reinitTimerReport(bool reset) {
//     if (reset) {
//         timerReport_Interval = 1 * 1000;
//     } else {
//         timerReport_Interval = 60 * 1000;
//     }    

//     timerReport_LastTime =  millis();    
// }

// void rfir::module::device::Device::doRawChanged() {
//     if (rawChanged && millis() - rawChanged_time > rawChanged_timeout) {
//         rawChanged = false;
//         saveRaw();
//     }
// }

// std::string rfir::module::device::Device::toBitString() {
//     int count = 0;
//     auto raw = getRaw(count);
//     return rfir::util::Util::BytesToString(raw, count);
// }

// std::string rfir::module::device::Device::toHexString() {
//     int count = 0;
//     auto raw = getRaw(count);
//     return rfir::util::Util::BytesToHexString(raw, count);
// }

//************************* V2 ***********************

bool rfir::module::device::Device::getCommonProps(neb::CJsonObject* pld){
    pld->ReplaceAdd("id", Config.dev_id);
    pld->ReplaceAdd("mac", rfir::util::Util::GetMacAddress());
    pld->ReplaceAdd("rssi", WiFi.RSSI());
    pld->ReplaceAdd("ip", "");
    pld->ReplaceAdd("ssid", "");
    pld->ReplaceAdd("version", OTA_VERSION_NUMBER);
    pld->ReplaceAdd("vendor", DEV_VENDOR);
    pld->ReplaceAdd("model", DEV_MODEL);    

    if (WiFi.isConnected()) {
        pld->ReplaceAdd("ip", WiFi.localIP().toString().c_str());
        pld->ReplaceAdd("ssid", WiFi.SSID().c_str());
    }
    return true;
};
bool rfir::module::device::Device::getProps(neb::CJsonObject* pld){
    auto result = onSvc_get(pld);
    getCommonProps(pld);
    return result;
};

bool rfir::module::device::Device::doEvtTimerReport(uint32_t timeout){
    GEventTimer.remove(m_timer_report_handler);
    m_timer_report_handler =  GEventTimer.delay(timeout, std::bind(&Device::onEvt_timer_report, this, std::placeholders::_1, std::placeholders::_2));
    return 1;
};

void* rfir::module::device::Device::onEvt_timer_report(void* arg, void* p){    
    std::string str = "Timer Report";
    events.onEvtPropsChange.emit((void*) str.c_str());
    doEvtTimerReport();
    return 0;
};

void* rfir::module::device::Device::doSvc_reboot(void* arg, void* p){    
    rfir::util::Util::Reset();
    return 0;
};


// bool rfir::module::device::Device::doEvt_props_change(const char* reason){
//     neb::CJsonObject pld;
//     getProps(&pld);
//     pld.ReplaceAdd("_reason", reason);
//     events.onEvtPropsChange.emit(&pld);
//     return 1;
// };

bool rfir::module::device::Device::onSvc_get(neb::CJsonObject* pld){
    return 0;
}; 

bool rfir::module::device::Device::onSvc_set(neb::CJsonObject* pld){
    return 0;
}; 

bool rfir::module::device::Device::onSvc_reboot(neb::CJsonObject* pld){
    int delay = 0;
    pld->Get("delay", delay);
    delay = std::max(100, delay);

    GEventTimer.delay(delay, std::bind(&Device::doSvc_reboot , this, std::placeholders::_1, std::placeholders::_2));
    return 1;
}; 

bool rfir::module::device::Device::onSvc_penet(neb::CJsonObject* pld){
    return 0;
};



#ifdef DEVICE_FZ_DM4_TEAROOM_AC_GREE
#include "rfir/module/device/ac/kelvinator/fz-dm4-tearoom-gree.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZDM4_TeaRoom_Gree();
#endif

//长乐DJ会议室Mcquay空调
#ifdef DEVICE_CL_DJROOM_AC_MCQUAY
#include "rfir/module/device/ac/mcquay/cl-djroom-mcquay.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_DJROOM_Mcquay();
#endif

//长乐DJ会议室杜亚窗帘
#ifdef DEVICE_CL_DJROOM_CURTAIN_DOOYA_485
#include "rfir/module/device/curtain/dooya/cl-djroom-dooya-485.h"
rfir::module::device::Device* GDevice = new rfir::module::device::curtain::CL_DJROOM_Dooya_485();
#endif

//长乐DJ会议室乐高区2路射灯开关
#ifdef DEVICE_CL_DJROOM_LIGHT_MSWITCH2
#include "rfir/module/device/mswitch/cl-djroom-light-mswitch2.h"
rfir::module::device::Device* GDevice = new rfir::module::device::mswitch::CL_DJROOM_Light_MSwitch2();
#endif


//长乐VR体验馆党建一楼柯耐弗空调风盘
#ifdef DEVICE_CL_VRHALL_FIRST_FLOOR_AC_OKONOFF
#include "rfir/module/device/ac/okonoff/cl-vrhall-ff-okonoff.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_VRHALL_FF_Okonoff();
#endif

//长乐VR体验馆党建一楼室外水机McQuay
#ifdef DEVICE_CL_VRHALL_F1_AC_MCQUAY_485
#include "rfir/module/device/ac/mcquay/cl-vrhall-f1-ac-mcquay-485.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_VRHALL_F1_MCQUAY_485();
#endif

//长乐VR体验馆二三楼Coolix空调
#ifdef DEVICE_CL_VRHALL_F2_AC_COOLIX
#include "rfir/module/device/ac/coolix/cl-vrhall-f2-coolix.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_VRHALL_F2_Coolix();
#endif

#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "rfir/module/device/speaker/sansui/cl-cxy-sansui-p300.h"
rfir::module::device::Device* GDevice = new rfir::module::device::speaker::CL_CXY_SANSUI_P300();
#endif

#ifdef DEVICE_TEST_SNIFFER_ESP32_WROOM
#include "rfir/module/device/test/sniffer/esp32_wroom.h"
rfir::module::device::Device* GDevice = new rfir::module::device::test::Sniffer_esp32_wroom();
#endif

//福州飘渺庄2楼食堂格力空调
#ifdef DEVICE_FZ_PMZ_F2_AC_GREE
#include "rfir/module/device/ac/gree/fz-pmz-f2-gree.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZ_PMZ_F2_Gree();
#endif

//福州飘渺庄2楼食堂格力空调
#ifdef DEVICE_FZ_DM3_AC_MEDIA
#include "rfir/module/device/ac/midea/fz-dm3-midea.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZ_DM3_Midea();
#endif

//Z3 协调器 ESP8266 + CC2652 
#ifdef DEVICE_Z3_COORDINATOR_ESP8266_CC2652
#include "rfir/module/device/z3/co/esp8266_cc2652.h"
rfir::module::device::Device* GDevice = new rfir::module::device::z3::co::ESP8266_CC2652();
#endif

