#include "device.h"
#include "rfir/rfir.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/interrupt.h"


void rfir::module::device::Device::init() {
    this->name = "device";  
}

void rfir::module::device::Device::start(void *) {
    init();
    loadConfig();   
}

void rfir::module::device::Device::loop() {

}

std::string rfir::module::device::Device::getConfigFilename() {
    return "/"+ this->name + "-config.json";
};

bool rfir::module::device::Device::loadConfig() {
    GInterrupt.stop();

    bool result = false;
    neb::CJsonObject json;
    std::string fn = getConfigFilename();
    rfir::util::TxtFile file(fn.c_str());
    std::string context;
    file.readString(context);    
    result = setConfig(context.c_str());

    GInterrupt.start();

    return result;
}

bool rfir::module::device::Device::saveConfig() {
    GInterrupt.stop();

    int result = 0;
    std::string context;    
    if (getConfig(context)) {
        neb::CJsonObject json;
        std::string fn = getConfigFilename();
        rfir::util::TxtFile file(fn.c_str());
        result =  file.writeString(context);
    }

    GInterrupt.start();
    
    return result != -1;
    
}

void rfir::module::device::Device::delaySaveConfig(int timeout_ms ){
     GEventTimer.delay(timeout_ms, std::bind(&Device::doSaveConfig, this, std::placeholders::_1, std::placeholders::_2));
}; 

void* rfir::module::device::Device::doSaveConfig(void* , void*){
    saveConfig();
    return 0;
};  

bool rfir::module::device::Device::setConfig(const char* context){
    return false;
};

bool rfir::module::device::Device::getConfig(std::string& context){
    return false;
};  

//************************* V2 ***********************

bool rfir::module::device::Device::getCommonProps(neb::CJsonObject* pld){
    pld->ReplaceAdd("id", Config.props.dev_id);
    pld->ReplaceAdd("mac", rfir::util::Util::GetMacAddress());
    pld->ReplaceAdd("rssi", WiFi.RSSI());
    pld->ReplaceAdd("ip", "");
    pld->ReplaceAdd("ssid", "");
    pld->ReplaceAdd("version", Config.props.ota_version_number);
    pld->ReplaceAdd("vendor", Config.props.dev_vendor);
    pld->ReplaceAdd("model", Config.props.dev_model);    

    if (WiFi.isConnected()) {
        pld->ReplaceAdd("ip", WiFi.localIP().toString().c_str());
        pld->ReplaceAdd("ssid", WiFi.SSID().c_str());
    }
    return true;
};
bool rfir::module::device::Device::getProps(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    auto result = onSvc_get(pld, cmd);
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


int rfir::module::device::Device::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::Device::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::Device::onSvc_config(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::Device::onSvc_reboot(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    DEBUGER.println("rfir::module::device::Device::onSvc_reboot");
    int delay = 0;
    pld->Get("delay", delay);
    delay = std::max(1000, delay);

    GEventTimer.delay(delay, std::bind(&Device::doSvc_reboot , this, std::placeholders::_1, std::placeholders::_2));
    return 1;
}; 

int rfir::module::device::Device::onSvc_penet(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
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

//长乐禅修院山水音箱
#ifdef DEVICE_CL_CXY_SPEAKER_SANSUI_P300
#include "rfir/module/device/speaker/sansui/cl-cxy-sansui-p300.h"
rfir::module::device::Device* GDevice = new rfir::module::device::speaker::CL_CXY_SANSUI_P300();
#endif

//长乐小镇美术馆格力空调485
#ifdef DEVICE_CL_XZ_MSG_AC_GREE_485
#include "rfir/module/device/ac/gree/cl-xz-msg-gree-485.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_XZ_MSG_GREE_485();
#endif

//长乐小镇美术馆格力空调485透传
#ifdef DEVICE_CL_XZ_MSG_AC_GREE_PENET
#include "rfir/module/device/ac/gree/cl-xz-msg-gree-penet.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::CL_XZ_MSG_GREE_PENET();
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

//福州亚太格力空调
#ifdef DEVICE_FZ_YT_AC_GREE
#include "rfir/module/device/ac/gree/fz-yt-gree.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZ_YT_Gree();
#endif

//福州东门美的空调
#ifdef DEVICE_FZ_DM_AC_MIDEA
#include "rfir/module/device/ac/midea/fz-dm-midea.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZ_DM_Midea();
#endif

//福州亚太美的空调
#ifdef DEVICE_FZ_YT_AC_MIDEA
#include "rfir/module/device/ac/midea/fz-yt-midea.h"
rfir::module::device::Device* GDevice = new rfir::module::device::ac::FZ_YT_Midea();
#endif

//Z3 协调器 ESP8266 + CC2652 
#ifdef DEVICE_Z3_COORDINATOR_ESP8266_CC2652
#include "rfir/module/device/z3/co/esp8266_cc2652.h"
rfir::module::device::Device* GDevice = new rfir::module::device::z3::co::ESP8266_CC2652();
#endif

//RS485-MODBUS
#ifdef DEVICE_RS485_PENET
#include "rfir/module/device/rs/rs-penet-485.h"
rfir::module::device::Device* GDevice = new rfir::module::device::RS::RSPenet485();
#endif