#include "ap.h"   
#include "client.h"
#include "rfir/util/util.h"
#include "rfir/util/led.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/serial.h"
#include "service/config.h"

network::module::wifi::AP::AP(){
    this->keyTimeBtnStart.pressed = 3000;
    this->keyTimeBtnStart.released = 5000;

    this->keyTimeBtnStop.pressed = 200;
    this->keyTimeBtnStop.released = 1000;
};
network::module::wifi::AP::~AP(){

};

void network::module::wifi::AP::start(void*) {
    initBtnStart(); 

    GConfig.events.ready.add(this,  [this](void*, void*)->void*{
        initWifiWatch();
        return 0;
    });    
    
    GWifiClient.events.softAPModeStationConnected.add(this, [this](void*, void*)->void*{
        if (!WiFi.isConnected()) {
            WiFi.disconnect(true);
            WiFi.disconnect(true);
        }
        return 0;
    });
}

void network::module::wifi::AP::start(){
    if (!started()) {
        init();        
        GLed.start(this->params.jled, this);    
    }
};

bool network::module::wifi::AP::started(){
    return this->iotWebConf;
};

void network::module::wifi::AP::stop(){
    GLed.stop(this);
    uninit();
};

void network::module::wifi::AP::loop() {
    if (iotWebConf)
        iotWebConf->doLoop();
}

void network::module::wifi::AP::initWifiWatch(){
    GWifiClient.events.onWifiConnect.remove(this);
    GWifiClient.events.onWifiDisconnect.remove(this);
    GWifiClient.events.onWifiConnect.add(this, [this](void*, void*)->void*{
        GEventTimer.remove(wifi_timeout_handler);
        wifi_timeout_handler = 0;
        this->stop();
        return 0;
    });

    GWifiClient.events.onWifiDisconnect.add(this, [this](void*, void*)->void*{
        if (!wifi_timeout_handler) {
            wifi_timeout_handler = GEventTimer.delay(this->params.start_wifi_timeout, [this](void* arg, void* p)->void*{ 
                wifi_timeout_handler = 0;
                this->start();
                return 0;
            });
        }
        return 0;
    });

    if (GWifiClient.params.ssid.size() == 0) {
        GEventTimer.delay(100, [this](void* arg, void* p)->void*{ 
            this->start();
            return 0;
        });
    }
};

void network::module::wifi::AP::init(){

    initWebConf();
    initWifi();
    initSerial();


    initBtnStop();
    GDebuger.println(F("AP Ready."));   
};

void network::module::wifi::AP::uninit(){
    if (iotWebConf) {
        uninitWebConf();  
        uninitWifi();
        uninitSerial();
        initBtnStart();    
        Serial.println(F("AP Released."));    
        rfir::util::Util::Reset(1000);
    }
};

void network::module::wifi::AP::initWebConf(){
    this->dnsServer = new DNSServer();
    this->webServer = new WebServer(80);
    this->iotWebConf = new IotWebConf(this->params.apSsid, this->dnsServer, this->webServer, this->params.apPass);

    Serial.print(F("AP Starting up...: "));
    Serial.print(this->params.apSsid);
    Serial.print(F(" "));
    Serial.println(this->params.apPass);
    
    this->iotWebConf->setApConnectionHandler([this](const char* apName, const char* password)->bool{return this->connectAp(apName, password);});
    this->iotWebConf->setConfigSavedCallback(std::bind(&AP::onConfigSaved, this));
    this->iotWebConf->getApPasswordParameter()->defaultValue = this->params.apPass;
    this->iotWebConf->getApPasswordParameter()->applyDefaultValue();

    auto valid = this->iotWebConf->init();
    #ifdef ESP8266
        WiFi.hostname(this->params.apSsid);
    #elif defined(ESP32)
        WiFi.setHostname(this->params.apSsid);
    #endif

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
    wifiParamPass = new IotWebConfTextParameter("wifi 密码", "wifi_pass", wifiPass, WifiSsid_max_length);

    wifiGroup->addItem(wifiParamSsid);
    wifiGroup->addItem(wifiParamPass);
    iotWebConf->addParameterGroup(wifiGroup);

};
void network::module::wifi::AP::uninitWifi(){
    delete wifiParamSsid;
    delete wifiParamPass;
    delete wifiGroup;
    delete wifiSsids;

    wifiSsids = 0;
    wifiGroup = 0;
    wifiParamSsid = 0;
    wifiParamPass = 0;
};
void network::module::wifi::AP::initSerial(){
    
    serialGroup = new IotWebConfParameterGroup("serial_setting", "串口配置");
    serialParamBand = new IotWebConfNumberParameter("波特率", "serial_band", serialBand, Serial_Band_max_length);    
    auto Serial_Config_max_count = GSerialConfigs.configs.getSize();
    serialConfigs = new char[Serial_Band_max_length * Serial_Config_max_count];
    serialParamConfig = new IotWebConfSelectParameter("数据位", "serial_config", serialConfig, Serial_Band_max_length, (char*)serialConfigs, (char*)serialConfigs, Serial_Config_max_count, Serial_Band_max_length);
    serialParamDebug = new IotWebConfCheckboxParameter("调试模式", "serial_debug", serialDebug, Serial_Band_max_length);

    serialGroup->addItem(serialParamBand);
    serialGroup->addItem(serialParamConfig);
    // serialGroup->addItem(serialParamDebug);
    iotWebConf->addParameterGroup(serialGroup);
  
};
void network::module::wifi::AP::uninitSerial(){
    delete serialConfigs;
    delete serialGroup;
    delete serialParamBand;
    delete serialParamConfig;
    delete serialParamDebug;

    serialConfigs = 0;
    serialGroup = 0;
    serialParamBand = 0;
    serialParamConfig = 0;  
    serialParamDebug = 0;  
};



void network::module::wifi::AP::initBtnStart(){
    GButton.events.onLongPressed.remove(this);
    GButton.events.onLongReleased.remove(this);
    GButton.events.onBetweenReleased.remove(this);

    GButton.events.onLongPressed.once(this, std::bind(&AP::onBtnLongPressed,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);
    GButton.events.onLongReleased.once(this, std::bind(&AP::onBtnLongReleased,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);
    GButton.events.onBetweenReleased.once(this, std::bind(&AP::onBtnStart,this, std::placeholders::_1, std::placeholders::_1), this, &this->keyTimeBtnStart);

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
    
    // String s = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1, user-scalable=no\"/>";
    // s += "<title>IotWebConf 01 Minimal</title></head><body>";
    // s += "Go to <a href='config'>configure page</a> to change settings.";
    // s += "</body></html>\n";

    // this->webServer->send(200, "text/html", s);

    this->iotWebConf->handleConfig();

}


void network::module::wifi::AP::applyDefault() {
    if ((int)this->events.applyDefault.emit(this) == -1)
        return;

    this->iotWebConf->getThingNameParameter()->defaultValue = this->params.apSsid;
    this->iotWebConf->getThingNameParameter()->applyDefaultValue();
    this->iotWebConf->getThingNameParameter()->label = 0;

    this->iotWebConf->getApPasswordParameter()->defaultValue = this->params.apPass;
    this->iotWebConf->getApPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getApPasswordParameter()->visible = false;


    // this->iotWebConf->getWifiSsidParameter()->defaultValue = this->params.wifiSsid;
    // this->iotWebConf->getWifiSsidParameter()->applyDefaultValue();
    this->iotWebConf->getWifiSsidParameter()->visible = false;

    // this->iotWebConf->getWifiPasswordParameter()->defaultValue = this->params.wifiPass;
    // this->iotWebConf->getWifiPasswordParameter()->applyDefaultValue();
    this->iotWebConf->getWifiPasswordParameter()->visible = false;


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

    auto map = GSerialConfigs.configs.getMap();
    int idx = 0;
    for (auto it = map->begin(); it != map->end(); it++)
    {
        strcpy(this->serialConfigs + idx * Serial_Band_max_length, it->second);    
        idx++;
    }

    serialParamBand->defaultValue = serialBand;
    serialParamBand->applyDefaultValue();
    serialParamConfig->defaultValue = serialConfig;
    serialParamConfig->applyDefaultValue();
    serialParamDebug->defaultValue = serialDebug;
    serialParamDebug->applyDefaultValue();
}


void* network::module::wifi::AP::onBtnStart(void* arg, void* p) {
    GDebuger.println(F("network::module::wifi::AP::onBtnStart"));
    GEventTimer.delay(100, [this](void* arg, void* p) -> void* {
        start();
        return 0;
    });        

    return 0;
};

void* network::module::wifi::AP::onBtnStop(void* arg, void* p) {
    GDebuger.println(F("network::module::wifi::AP::onBtnStop"));
    GEventTimer.delay(100, [this](void* arg, void* p) -> void* {
        stop();
        return 0;
    });

    return 0;
};

void* network::module::wifi::AP::onBtnLongPressed(void* arg, void* p) {
    GDebuger.println(F("network::module::wifi::AP::onBtnLongPressed"));
    GLed.start(this->params.jled, this);  
    return 0;
};

void* network::module::wifi::AP::onBtnLongReleased(void* arg, void* p) {
    GDebuger.println(F("network::module::wifi::AP::onBtnLongReleased"));
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
    return WiFi.softAP(this->params.apSsid, this->params.apPass);
};

void network::module::wifi::AP::onConfigSaved()
{
    Serial.println(F("network::module::wifi::AP::onConfigSaved"));
    this->events.configSaved.emit(this);
}        

network::module::wifi::AP GWifiAP;
