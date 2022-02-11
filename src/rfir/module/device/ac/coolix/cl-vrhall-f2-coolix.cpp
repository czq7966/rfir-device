#include "cl-vrhall-f2-coolix.h"
#include "../../../../service/device/device.h"
#include "global.h"


rfir::module::ttl::Config::Device* rfir::module::device::ac::CL_VRHALL_F2_Coolix::init() {
    auto d = Coolix::init();
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


void rfir::module::device::ac::CL_VRHALL_F2_Coolix::start(void * p) {
    Coolix::start(p);

}


void rfir::module::device::ac::CL_VRHALL_F2_Coolix::loop() {
    Coolix::loop();
}

bool rfir::module::device::ac::CL_VRHALL_F2_Coolix::onCmd_get(neb::CJsonObject* pld) {
    Coolix::onCmd_get(pld);
    float iaValue = 0;
    auto running = isRunning(iaValue);
    std::string runningStr = running ? "on" : "off";
    pld->Add("running", runningStr);
    pld->Add("iaValue", iaValue);
    return true;
}


bool rfir::module::device::ac::CL_VRHALL_F2_Coolix::waitStart(int timeout) {
    static unsigned long CL_VRHALL_F2_Coolix_waitStart_time = 0;
    static bool CL_VRHALL_F2_Coolix_waitStart_result = false;
    if (!CL_VRHALL_F2_Coolix_waitStart_result) {
        if (CL_VRHALL_F2_Coolix_waitStart_time == 0)
            CL_VRHALL_F2_Coolix_waitStart_time = millis();
        if (((millis() - CL_VRHALL_F2_Coolix_waitStart_time) > timeout))
            CL_VRHALL_F2_Coolix_waitStart_result = true;

    }

    return CL_VRHALL_F2_Coolix_waitStart_result;

}

bool rfir::module::device::ac::CL_VRHALL_F2_Coolix::isRunning(float& iaValue) {
    return false;
}

void rfir::module::device::ac::CL_VRHALL_F2_Coolix::syncPower() {
    float iaValue = 0;
    auto running = isRunning(iaValue);
    this->ac->setPower(running);    
    int count = 0;     
    this->setRaw(this->getRaw(count));    
}

bool rfir::module::device::ac::CL_VRHALL_F2_Coolix::isPowerOff() {
    float iaValue = 0;
    return !this->ac->getPower() && !isRunning(iaValue);
}

void rfir::module::device::ac::CL_VRHALL_F2_Coolix::powerOff() {
    this->ac->setPower(false);
    rfir::service::device::Device::SendEncodeRaw();   
    int count = 0;     
    this->setRaw(this->getRaw(count));
}

//cancel = reset == true && timeout <= 0
int rfir::module::device::ac::CL_VRHALL_F2_Coolix::powerOffCountdown(bool reset, int timeout ) {
    static unsigned long CL_VRHALL_F2_Coolix_powerOffCountdown_time = 0;
    static bool CL_VRHALL_F2_Coolix_powerOffCountdown_ing = false;
    static int  CL_VRHALL_F2_Coolix_powerOffCountdown_predown = 0;

    if (reset) {
        if (timeout > 0) {
            CL_VRHALL_F2_Coolix_powerOffCountdown_time = millis();
            CL_VRHALL_F2_Coolix_powerOffCountdown_ing = true;
            //关机倒记时
            emitChange(("power off count down: " + String(timeout / 1000)).c_str());
        } else {
            CL_VRHALL_F2_Coolix_powerOffCountdown_time = 0;
            CL_VRHALL_F2_Coolix_powerOffCountdown_ing = false;
        }
    }

    if (CL_VRHALL_F2_Coolix_powerOffCountdown_ing) {
        int down = timeout - (millis() - CL_VRHALL_F2_Coolix_powerOffCountdown_time);
        if (down <= 0 || isPowerOff() ) {            
            CL_VRHALL_F2_Coolix_powerOffCountdown_ing = false;            
            //关机成功
            if (isPowerOff()) {
                emitChange("power off success!");
            } else { //关机失败，恢复电源状态位
                syncPower();
                emitChange("power off failed!");
            }
        } else {
            //todo emitChange(...tick down);
            // int countdown = down / 1000;
            // if (countdown != CL_VRHALL_F2_Coolix_powerOffCountdown_predown) {
            //     CL_VRHALL_F2_Coolix_powerOffCountdown_predown = countdown;                
            //     emitChange(("power off count down: " + String(countdown)).c_str());
            // }

            return down;
        }

    }

    return CL_VRHALL_F2_Coolix_powerOffCountdown_ing;
}

void rfir::module::device::ac::CL_VRHALL_F2_Coolix::On_HLW8110_IA_Switch(int arg, bool ison) {
    auto device = (CL_VRHALL_F2_Coolix*)arg;
    if (device) {
        device->emitChange("On IA Switch");
    }
}