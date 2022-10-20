#include "client.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "rfir/util/debuger.h"


void network::module::wifi::Client::start() {
    //V2
    startV2();    
}

void network::module::wifi::Client::loop() {
    //V2
    loopV2();
}



//V2

void  network::module::wifi::Client::startV2(){
#ifdef ESP8266    
    wifiConnectHandler = WiFi.onStationModeGotIP(std::bind(&Client::_onWifiConnect,this, std::placeholders::_1));
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(std::bind(&Client::_onWifiDisconnect,this, std::placeholders::_1));
    softAPModeStationConnectedHandler = WiFi.onSoftAPModeStationConnected(std::bind(&Client::_onSoftAPModeStationConnected, this, std::placeholders::_1));
    softAPModeStationDisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(std::bind(&Client::_onSoftAPModeStationDisconnected,this, std::placeholders::_1));

#else
    WiFi.onEvent(std::bind(&Client::WiFiEvent, this, std::placeholders::_1));
#endif    
    events.onWifiConnect.add(this, std::bind(&Client::onWifiConnect, this, std::placeholders::_1, std::placeholders::_2));
    events.onWifiDisconnect.add(this, std::bind(&Client::onWifiDisconnect, this, std::placeholders::_1, std::placeholders::_2));
    delayConnectToWifi(500);
};
void  network::module::wifi::Client::loopV2(){
};

void  network::module::wifi::Client::connectToWifi(){
    removeDelayConnectHandler();

    if (WiFi.isConnected()) 
        return;
    
    if (WiFi.softAPgetStationNum() > 0) {
        delayConnectToNextWifi();
        return;
    }

    if (m_connect_timeout_handler == 0) {        
        m_connect_timeout_handler = GEventTimer.delay(params.timeout, std::bind(&Client::onWifiConnectTimeout, this, std::placeholders::_1, std::placeholders::_2));
    }

    if (GLed.idle()) GLed.start(this->params.jled, this);
   
    if (m_connect_ssid_index < params.ssid.size()) {
        auto ssid_ssid = this->params.ssid[m_connect_ssid_index];
        auto ssid_pass = this->params.pass[m_connect_ssid_index];
        GDebuger.print(F("Wifi connecting... "));
        GDebuger.print(ssid_ssid);
        GDebuger.print(F(" "));
        GDebuger.println(ssid_pass);
        WiFi.begin(ssid_ssid, ssid_pass);        
    }    

    delayConnectToNextWifi();
};

void  network::module::wifi::Client::connectToNextWifi() {
    m_connect_ssid_index++;
    if (m_connect_ssid_index < 0 || m_connect_ssid_index >= params.ssid.size()) {
        m_connect_ssid_index = 0;
    }
    connectToWifi();
}

void  network::module::wifi::Client::removeDelayConnectHandler() {
    GEventTimer.remove(m_delay_connect_handler);
    m_delay_connect_handler = 0;    
};

void  network::module::wifi::Client::removeConnectTimeoutHandler() {
    GEventTimer.remove(m_connect_timeout_handler);    
    m_connect_timeout_handler = 0;  
};




void  network::module::wifi::Client::delayConnectToWifi(int timeoutms){
    if (m_delay_connect_handler == 0)
        m_delay_connect_handler = GEventTimer.delay(timeoutms, std::bind(&Client::doConnectToWifi, this, std::placeholders::_1, std::placeholders::_2));
};

void  network::module::wifi::Client::delayConnectToNextWifi(int timeoutms){
    if (m_delay_connect_handler == 0)
        m_delay_connect_handler = GEventTimer.delay(timeoutms, std::bind(&Client::doConnectToNextWifi, this, std::placeholders::_1, std::placeholders::_2));
};

void* network::module::wifi::Client::doConnectToWifi(void* arg, void* p) {
    connectToWifi();
    return 0;
}

void* network::module::wifi::Client::doConnectToNextWifi(void* arg, void* p) {
    connectToNextWifi();
    return 0;
}

#ifdef ESP8266
void network::module::wifi::Client::_onWifiConnect(const WiFiEventStationModeGotIP& event) {
    this->events.onWifiConnect.emit(0);
};

void network::module::wifi::Client::_onWifiDisconnect(const WiFiEventStationModeDisconnected& event){
    this->events.onWifiDisconnect.emit(0);
};


void network::module::wifi::Client::_onSoftAPModeStationConnected(const WiFiEventSoftAPModeStationConnected& event){
    this->events.softAPModeStationConnected.emit(0);
};
void network::module::wifi::Client::_onSoftAPModeStationDisconnected(const WiFiEventSoftAPModeStationDisconnected& event){
    this->events.softAPModeStationDisconnected.emit(0);
};

#else
void network::module::wifi::Client::WiFiEvent(WiFiEvent_t event) {
    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        this->events.onWifiConnect.emit(0);
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        this->events.onWifiDisconnect.emit(0);
        break;
    }
}
#endif


void* network::module::wifi::Client::onWifiConnect(void* arg, void* p){
    GDebuger.print(F("Wifi connected to : "));
    GDebuger.println(WiFi.SSID());

    removeConnectTimeoutHandler();
    removeDelayConnectHandler();

    onWifiCheckTimeout(0, 0);
    GLed.stop(this);        
    return 0;
};
void* network::module::wifi::Client::onWifiDisconnect(void* arg, void* p) {
    GDebuger.print(F("Wifi disconnect from :"));
    GDebuger.println(WiFi.SSID());
    if (m_delay_connect_handler == 0) {
        delayConnectToWifi();    
    }
    return 0;
};

void* network::module::wifi::Client::onWifiConnectTimeout(void* arg, void* p){
    rfir::util::Util::Reset();
    return 0;
};

void* network::module::wifi::Client::onWifiCheckTimeout(void* arg, void* p){
    if (!WiFi.isConnected() && m_connect_timeout_handler == 0) {
        rfir::util::Util::Reset();
    }

    GEventTimer.remove(m_check_timeout_handler);
    m_check_timeout_handler = GEventTimer.delay(params.interval, std::bind(&Client::onWifiCheckTimeout, this, std::placeholders::_1, std::placeholders::_2));
    return 0;
};

void network::module::wifi::Client::clearSsids(){
    while (this->params.ssid.size() > 0) {
        auto ssid = this->params.ssid.front();
        this->params.ssid.erase(this->params.ssid.begin());
        delete ssid;
    }

    while (this->params.pass.size() > 0) {
        auto pass = this->params.pass.front();
        this->params.pass.erase(this->params.pass.begin());
        delete pass;
    }

};

void network::module::wifi::Client::addSsids(const char* ssids, const char* passes){
    
    int len = strlen(ssids);
    int offset = 0;
    int count = 0;
    char ssid[33]; memset(ssid, 0, 33);
    for (size_t i = 0; i < len; i++)
    {
        char c = ssids[i];
        if (c == ',' || c == ' ' || i == len - 1){            
            if (i == len -1) ssid[i - offset] = c; 
            auto newSsid = new char[33];
            strcpy(newSsid, ssid);
            this->params.ssid.push_back(newSsid);
            count++;
            memset(ssid, 0, 33);
            offset = i + 1;
        }
        else 
            ssid[i - offset] = c;        
    }
    
    len = strlen(passes);
    offset = 0;
    char pass[33]; memset(pass, 0, 33); 
    for (size_t i = 0; i < len; i++)
    {
        char c = passes[i];
        if (c == ',' || c == ' ' || i == len - 1){            
            if (i == len -1) pass[i - offset] = c; 
            auto newPass = new char[33];
            strcpy(newPass, pass);
            this->params.pass.push_back(newPass);
            memset(pass, 0, 33); 
            offset = i + 1;
        }
        else 
            pass[i - offset] = c;        
    }
    

};

void network::module::wifi::Client::addSsid(const char* ssid, const char* pass){
    this->params.ssid.push_back(ssid);
    this->params.pass.push_back(pass);
};


network::module::wifi::Client GWifiClient;
