#include "cl-vrhall-ff-okonoff.h"
#include "../../../sensor/hlw8110/hlw8110.h"
#include "../../../../service/device/device.h"
#include "global.h"


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

    sensor::HLW8110::Start();
    sensor::HLW8110::On_IA_Switch = On_HLW8110_IA_Switch;    
    sensor::HLW8110::IA_Switch_CB_Arg = (int)this;    

    //硬重启？
    if (Global::IsPowerHardReset()) {
        syncPower();       
    }   
}


void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::loop() {
    Okonoff::loop();
    sensor::HLW8110::Loop();

    //已等待2秒
    if (waitStart(2000)) {
        //非在倒计时
        if (!powerOffCountdown()) {
            //未关机
            if (!isPowerOff()) {
                //要关机
                if(!this->ac->getPower()) {
                    //发送关机指令
                    powerOff();
                    //启动关机倒计时
                    powerOffCountdown(true);
                }
            }
        }    
    }
}

bool rfir::module::device::ac::CL_VRHALL_FF_Okonoff::onCmd_get(neb::CJsonObject* pld) {
    Okonoff::onCmd_get(pld);
    float iaValue = 0;
    auto running = isRunning(iaValue);
    std::string runningStr = running ? "on" : "off";
    pld->Add("running", runningStr);
    pld->Add("iaValue", iaValue);
    return true;
}


bool rfir::module::device::ac::CL_VRHALL_FF_Okonoff::waitStart(int timeout) {
    static unsigned long CL_VRHALL_FF_Okonoff_waitStart_time = 0;
    static bool CL_VRHALL_FF_Okonoff_waitStart_result = false;
    if (!CL_VRHALL_FF_Okonoff_waitStart_result) {
        if (CL_VRHALL_FF_Okonoff_waitStart_time == 0)
            CL_VRHALL_FF_Okonoff_waitStart_time = millis();
        if (((millis() - CL_VRHALL_FF_Okonoff_waitStart_time) > timeout))
            CL_VRHALL_FF_Okonoff_waitStart_result = true;

    }

    return CL_VRHALL_FF_Okonoff_waitStart_result;

}

bool rfir::module::device::ac::CL_VRHALL_FF_Okonoff::isRunning(float& iaValue) {
    iaValue = sensor::HLW8110::Hlw8110_Get_Current();
    return iaValue >= sensor::HLW8110::IA_Switch_Point;
}

void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::syncPower() {
    float iaValue = 0;
    auto running = isRunning(iaValue);
    this->ac->setPower(running);    
}

bool rfir::module::device::ac::CL_VRHALL_FF_Okonoff::isPowerOff() {
    float iaValue = 0;
    return !this->ac->getPower() && !isRunning(iaValue);
}

void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::powerOff() {
    this->ac->setPower(false);
    rfir::service::device::Device::SendEncodeRaw();   
    int count = 0;     
    this->setRaw(this->getRaw(count));
}

//cancel = reset == true && timeout <= 0
int rfir::module::device::ac::CL_VRHALL_FF_Okonoff::powerOffCountdown(bool reset, int timeout ) {
    static unsigned long CL_VRHALL_FF_Okonoff_powerOffCountdown_time = 0;
    static bool CL_VRHALL_FF_Okonoff_powerOffCountdown_ing = false;

    if (reset) {
        if (timeout > 0) {
            CL_VRHALL_FF_Okonoff_powerOffCountdown_time = millis();
            CL_VRHALL_FF_Okonoff_powerOffCountdown_ing = true;
        } else {
            CL_VRHALL_FF_Okonoff_powerOffCountdown_time = 0;
            CL_VRHALL_FF_Okonoff_powerOffCountdown_ing = false;
        }
    }

    if (CL_VRHALL_FF_Okonoff_powerOffCountdown_ing) {
        int down = timeout - (millis() - CL_VRHALL_FF_Okonoff_powerOffCountdown_time);
        if (down <= 0 || isPowerOff() ) {            
            CL_VRHALL_FF_Okonoff_powerOffCountdown_ing = false;            
            //关机成功
            if (isPowerOff()) {
                emitChange("power off success!");
            } else { //关机失败
                //todo 

                emitChange("power off failed!");
            }
        } else {
            //todo emitChange(...tick down);
            return down;
        }

    }

    return CL_VRHALL_FF_Okonoff_powerOffCountdown_ing;
}

void rfir::module::device::ac::CL_VRHALL_FF_Okonoff::On_HLW8110_IA_Switch(int arg, bool ison) {
    auto device = (CL_VRHALL_FF_Okonoff*)arg;
    if (device) {
        device->emitChange("On IA Switch");
    }
}