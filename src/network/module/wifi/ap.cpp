#include "ap.h"   
#include "config.h"        
#include "rfir/util/util.h"
#include "rfir/util/led.h"
#include "rfir/util/event-timer.h"

void network::module::wifi::AP::init(){
    this->dnsServer = new DNSServer();
    this->webServer = new WebServer(80);
    this->iotWebConf = new IotWebConf(this->params.apSsid.c_str(), this->dnsServer, this->webServer, this->params.apPass.c_str(), this->params.configVersion.c_str());

    DEBUGER.printf("AP Starting up...%s %s\n:", this->params.apSsid.c_str(), this->params.apPass.c_str());
    this->iotWebConf->setStatusPin(this->params.statusPin, this->params.statusPinOnLevel);
    this->iotWebConf->setConfigPin(this->params.configPin);
    this->iotWebConf->setWifiConnectionHandler(&connectWifi);
    this->iotWebConf->setApConnectionHandler(connectAp);
    this->iotWebConf->setConfigSavedCallback([this]{this->events.configSaved.emit(this);});

    auto valid = this->iotWebConf->init();
    this->iotWebConf->forceApMode(true);

    // this->applyDefault();    
    this->webServer->on("/", [this]{ this->handleRoot(); });
    this->webServer->on("/config", [this]{ this->iotWebConf->handleConfig(); });
    this->webServer->onNotFound([this]{ this->iotWebConf->handleNotFound(); });

    DEBUGER.println("AP Ready.");

    initBtnStop();
};

void network::module::wifi::AP::uninit(){

    if (iotWebConf) 
        WiFi.softAPdisconnect(true);            
    DEBUGER.println("111111111111");
        
    if(webServer)
        webServer->stop();
DEBUGER.println("22222222222");
    if (dnsServer)
        dnsServer->stop();
DEBUGER.println("3333333333");    
    delete iotWebConf;
    delete webServer;
    delete dnsServer;
DEBUGER.println("44444444444");

    iotWebConf = 0;
    webServer = 0;
    dnsServer = 0;
DEBUGER.println("5555555555555");
    unApplyDefault();
DEBUGER.println("66666666666");    
    initBtnStart();    
DEBUGER.println("77777777777");    
};

void network::module::wifi::AP::initBtnStart(){
    GButton.events.onLongPressed.remove(this);
    GButton.events.onLongReleased.remove(this);
    GButton.events.onBetweenReleased.remove(this);

    GButton.events.onLongPressed.once(this, std::bind(&AP::onBtnLongPressed,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);
    GButton.events.onLongReleased.once(this, std::bind(&AP::onBtnLongReleased,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);
    GButton.events.onBetweenReleased.once(this, std::bind(&AP::onBtnStart,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);

};

void* network::module::wifi::AP::delayInitBtnStart(void* arg, void* p){
    return 0;

};

void network::module::wifi::AP::initBtnStop(){
    GButton.events.onLongPressed.remove(this);
    GButton.events.onLongReleased.remove(this);
    GButton.events.onBetweenReleased.remove(this);

    GButton.events.onReleased.remove(this);
    GButton.events.onReleased.once(this, std::bind(&AP::onBtnStop,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStop);
};

void network::module::wifi::AP::start(Params p) {
    this->params = p;
    stop();
}

void network::module::wifi::AP::start(){
    init();
    if (GLed.idle()) GLed.start(&(AP_CONFIG_JLED), this);    
};

void network::module::wifi::AP::stop(){
    GLed.stop(this);
    uninit();
};

void network::module::wifi::AP::loop() {
    if (iotWebConf)
        iotWebConf->doLoop();
}

void network::module::wifi::AP::handleRoot() {

    if (this->iotWebConf->handleCaptivePortal())
    {
        // -- Captive portal request were already served.
        return;
    }
    applyDefault();

    String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    s += "<title>IotWebConf 01 Minimal</title></head><body>";
    s += "Go to <a href='config'>configure page</a> to change settings.";
    s += "</body></html>\n";

    this->webServer->send(200, "text/html", s);
}


void network::module::wifi::AP::applyDefault() {
    unApplyDefault();

    this->iotWebConf->getThingNameParameter()->defaultValue = this->params.apSsid.c_str();
    this->iotWebConf->getThingNameParameter()->applyDefaultValue();
    this->iotWebConf->getThingNameParameter()->label = 0;
    // this->iotWebConf->getThingNameParameter()->visible = false;
    

    this->iotWebConf->getApPasswordParameter()->defaultValue = this->params.apPass.c_str();
    this->iotWebConf->getApPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getApPasswordParameter()->visible = false;

    this->iotWebConf->getWifiSsidParameter()->defaultValue = this->params.wifiSsid.c_str();
    this->iotWebConf->getWifiSsidParameter()->applyDefaultValue();
    this->iotWebConf->getWifiSsidParameter()->visible = false;

    this->iotWebConf->getWifiPasswordParameter()->defaultValue = this->params.wifiPass.c_str();
    this->iotWebConf->getWifiPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getWifiPasswordParameter()->visible = false;

    this->events.applyDefault.emit(this);
    DEBUGER.println("Use defaults: ");
    DEBUGER.printf("---AP   Ssid: %s\n", this->params.apSsid.c_str());
    DEBUGER.printf("---AP   Pass: %s\n", this->params.apPass.c_str());
    DEBUGER.printf("---WiFi Ssid: %s\n", this->params.wifiSsid.c_str());
    DEBUGER.printf("---WiFi Pass: %s\n", this->params.wifiPass.c_str());

    int count = WiFi.scanNetworks();    
    wifiSsids = new char[WifiSsid_max_length * WifiSsid_max_count];

    for(int i=0; i < WifiSsid_max_count; i++){             
        if (i < count) {            
            strcpy(this->wifiSsids + i * WifiSsid_max_length, WiFi.SSID(i).c_str());
        }
    }

    WiFi.scanDelete();
    IotWebConfParameterGroup* wifiGroup = new IotWebConfParameterGroup("wifi_factor", "Wifi Factor");
    IotWebConfSelectParameter* wifiParam = new IotWebConfSelectParameter("wifi ssid", "wifi_ssid", wifiSsid, WifiSsid_max_length, (char*)wifiSsids, (char*)wifiSsids, count, WifiSsid_max_length);
    wifiParam.
    wifiGroup->addItem(wifiParam);
    iotWebConf->addParameterGroup(wifiGroup);
    iotWebConf->
}

void network::module::wifi::AP::unApplyDefault() {
    // for(int i=0; i < WifiSsid_max_count; i++){ delete[] this->wifiSsids[i]; this->wifiSsids[i] = 0; }
    delete wifiSsids;
    delete wifiGroup;
    delete wifiParam;
    wifiSsids = 0;
    wifiGroup = 0;
    wifiParam = 0;
}


void* network::module::wifi::AP::onBtnStart(void* arg, void* p) {
    DEBUGER.println("network::module::wifi::AP::onBtnStart");
    GEventTimer.delay(100, [this](void* arg, void* p) -> void* {
        start();
        return 0;
    });        

    return 0;
};

void* network::module::wifi::AP::onBtnStop(void* arg, void* p) {
    DEBUGER.println("network::module::wifi::AP::onBtnStop");
    GEventTimer.delay(100, [this](void* arg, void* p) -> void* {
        stop();
        return 0;
    });

    return 0;
};

void* network::module::wifi::AP::onBtnLongPressed(void* arg, void* p) {
    DEBUGER.println("network::module::wifi::AP::onBtnLongPressed");
    if (GLed.idle()) GLed.start(&(AP_CONFIG_JLED), this);    
    return 0;
};

void* network::module::wifi::AP::onBtnLongReleased(void* arg, void* p) {
    DEBUGER.println("network::module::wifi::AP::onBtnLongReleased");
    GLed.stop(this);
    GEventTimer.delay(1000, [this](void* arg, void* p) -> void* {
        this->initBtnStart(); 
        return 0;
    });
    
    return 0;
};

void network::module::wifi::AP::connectWifi(const char* ssid, const char* password)
{
    WiFi.begin(ssid, password);
}

bool network::module::wifi::AP::connectAp(const char* apName, const char* password){
    WiFi.mode(WIFI_AP_STA);
    return WiFi.softAP(apName, password);
};

network::module::wifi::AP GWifiAP;