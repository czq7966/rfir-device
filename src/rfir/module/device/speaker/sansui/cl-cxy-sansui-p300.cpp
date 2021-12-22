#include "cl-cxy-sansui-p300.h"


bool rfir::module::device::speaker::CL_CXY_SANSUI_P300::PlayPinChanged = false;
void rfir::module::device::speaker::CL_CXY_SANSUI_P300::start(void * p) {
    SANSUI::start(p);
    gpioPlay.init(PIN_PLAY, INPUT);
    gpioPlay.onChange = OnPlayPinChange;
    gpioPlay.start();
};

void rfir::module::device::speaker::CL_CXY_SANSUI_P300::loop() {
    SANSUI::loop();
    doModePinChange();
};       

rfir::module::ttl::Config::Device* rfir::module::device::speaker::CL_CXY_SANSUI_P300::init() {
    auto d = SANSUI::init();
    if (!d) return 0;
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = PIN_SNIFF;
    // 发码参数 
    d->packet.send.params.pin = PIN_SEND;
#endif    
    return d;
}


bool rfir::module::device::speaker::CL_CXY_SANSUI_P300::onCmd_get(neb::CJsonObject* pld) {
    auto r = SANSUI::onCmd_get(pld);

    pld->Add("play", this->gpioPlay.read());

    
    return true;
};

bool rfir::module::device::speaker::CL_CXY_SANSUI_P300::doModePinChange() {
    if (PlayPinChanged) {
        PlayPinChanged = false;
        emitChange("Play state change");
        return true;
    }

    return false;


};

void rfir::module::device::speaker::CL_CXY_SANSUI_P300::OnPlayPinChange(rfir::module::ttl::Gpio* gpio, int value) {
    PlayPinChanged = true;
};