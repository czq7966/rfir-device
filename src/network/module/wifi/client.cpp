#include "client.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "rfir/util/debuger.h"


bool network::module::wifi::Client::Params::assign(Params& p) {
    this->ssid.assign(p.ssid.begin(), p.ssid.end());
    this->pass.assign(p.pass.begin(), p.pass.end());
    this->timeout = p.timeout;
    return true;
}

void network::module::wifi::Client::start(Params& p) {
    this->params.assign(p);
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
        GDebuger.printf("Wifi connecting... %s %s \r\n", ssid_ssid, ssid_pass);
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
    GDebuger.printf("Wifi connected to : %s \r\n", WiFi.SSID());
    removeConnectTimeoutHandler();
    removeDelayConnectHandler();

    onWifiCheckTimeout(0, 0);
    GLed.stop(this);        
    return 0;
};
void* network::module::wifi::Client::onWifiDisconnect(void* arg, void* p) {
    GDebuger.printf("Wifi disconnect from : %s \r\n", WiFi.SSID());
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

network::module::wifi::Client GWifiClient;
