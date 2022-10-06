#include "ap.h"   
#include "config.h"        
#include "rfir/util/util.h"
#include "rfir/util/led.h"
#include "rfir/util/event-timer.h"

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

void network::module::wifi::AP::init(){
    initWebConf();
    initWifi();
    initSerial();



    DEBUGER.println("AP Ready.");

    initBtnStop();
};

void network::module::wifi::AP::uninit(){

    uninitWebConf();  
    uninitWifi();
    uninitSerial();
    initBtnStart();    
};

void network::module::wifi::AP::initWebConf(){
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

    this->webServer->on("/", [this]{ this->handleRoot(); });
    this->webServer->on("/config", [this]{ this->iotWebConf->handleConfig(); });
    this->webServer->onNotFound([this]{ this->iotWebConf->handleNotFound(); });
};

void network::module::wifi::AP::uninitWebConf(){
    if (iotWebConf) 
        WiFi.softAPdisconnect(true);            
        
    if(webServer)
        webServer->stop();
    if (dnsServer)
        dnsServer->stop();
 
    delete iotWebConf;
    delete webServer;
    delete dnsServer;

    iotWebConf = 0;
    webServer = 0;
    dnsServer = 0;
};

void network::module::wifi::AP::initWifi(){
    wifiSsids = new char[WifiSsid_max_length * WifiSsid_max_count];

    wifiGroup = new IotWebConfParameterGroup("wifi_factor", "Wifi配网");
    wifiParamSsid = new IotWebConfSelectParameter("wifi 名称", "wifi_ssid", wifiSsid, WifiSsid_max_length, (char*)wifiSsids, (char*)wifiSsids, WifiSsid_max_count, WifiSsid_max_length);
    wifiParamPass = new IotWebConfPasswordParameter("wifi 密码", "wifi_pass", wifiPass, WifiSsid_max_length);
    wifiParamPass->defaultValue = this->params.wifiPass.c_str();
    wifiParamPass->applyDefaultValue();

    wifiGroup->addItem(wifiParamSsid);
    wifiGroup->addItem(wifiParamPass);
    iotWebConf->addParameterGroup(wifiGroup);

};
void network::module::wifi::AP::uninitWifi(){
    delete wifiSsids;
    delete wifiGroup;
    delete wifiParamSsid;
    delete wifiParamPass;

    wifiSsids = 0;
    wifiGroup = 0;
    wifiParamSsid = 0;
    wifiParamPass = 0;
};
void network::module::wifi::AP::initSerial(){
    serialConfigs = new char[Serial_Band_max_length * Serial_Config_max_count];
    serialGroup = new IotWebConfParameterGroup("serial_factor", "串口pz");
    serialParamBand = new IotWebConfNumberParameter("波特率", "serial_band", serialBand, Serial_Band_max_length);
    serialParamConfig = new IotWebConfSelectParameter("控制位", "serial_config", serialConfig, WifiSsid_max_length, (char*)serialConfigs, (char*)serialConfigs, Serial_Config_max_count, Serial_Band_max_length);
  
    serialParamBand->defaultValue = this->serialBand;
    serialParamBand->applyDefaultValue();
    serialParamConfig->defaultValue = this->serialConfig;
    serialParamConfig->applyDefaultValue();

    serialGroup->addItem(serialParamBand);
    serialGroup->addItem(serialParamConfig);
    iotWebConf->addParameterGroup(serialGroup);
};
void network::module::wifi::AP::uninitSerial(){
    delete serialConfigs;
    delete serialGroup;
    delete serialParamBand;
    delete serialParamConfig;

    serialConfigs = 0;
    serialGroup = 0;
    serialParamBand = 0;
    serialParamConfig = 0;    
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
    if ((int)this->events.applyDefault.emit(this) == -1)
        return;

    this->iotWebConf->getThingNameParameter()->defaultValue = this->params.apSsid.c_str();
    this->iotWebConf->getThingNameParameter()->applyDefaultValue();
    this->iotWebConf->getThingNameParameter()->label = 0;

    this->iotWebConf->getApPasswordParameter()->defaultValue = this->params.apPass.c_str();
    this->iotWebConf->getApPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getApPasswordParameter()->visible = false;

    this->iotWebConf->getWifiSsidParameter()->defaultValue = this->params.wifiSsid.c_str();
    this->iotWebConf->getWifiSsidParameter()->applyDefaultValue();
    this->iotWebConf->getWifiSsidParameter()->visible = false;

    this->iotWebConf->getWifiPasswordParameter()->defaultValue = this->params.wifiPass.c_str();
    this->iotWebConf->getWifiPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getWifiPasswordParameter()->visible = false;

    DEBUGER.println("Use defaults: ");
    DEBUGER.printf("---AP   Ssid: %s\n", this->params.apSsid.c_str());
    DEBUGER.printf("---AP   Pass: %s\n", this->params.apPass.c_str());
    DEBUGER.printf("---WiFi Ssid: %s\n", this->params.wifiSsid.c_str());
    DEBUGER.printf("---WiFi Pass: %s\n", this->params.wifiPass.c_str());

    int count = WiFi.scanNetworks();    
    for(int i=0; i < WifiSsid_max_count; i++){         
        char * ssid = this->wifiSsids + i * WifiSsid_max_length;    
        if (i < count)            
            strcpy(ssid, WiFi.SSID(i).c_str());
        else 
            strcpy(ssid, "");
    }

    WiFi.scanDelete();

    wifiParamSsid->defaultValue = wifiSsid;
    wifiParamSsid->applyDefaultValue();
    wifiParamPass->defaultValue = wifiPass;
    wifiParamPass->applyDefaultValue();

    serialParamBand->defaultValue = serialBand;
    serialParamBand->applyDefaultValue();
    serialParamConfig->defaultValue = serialConfig;
    serialParamConfig->applyDefaultValue();
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