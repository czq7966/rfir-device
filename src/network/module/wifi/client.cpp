#include "client.h"
#include "config.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "rfir/util/led.h"


bool network::module::wifi::Client::Params::assign(Params& p) {
    // this->apMode = p.apMode;
    // this->ap = p.ap;
    // this->smcMode = p.smcMode;
    // this->smc = p.smc;
    // this->smc.parent = this;
    this->ssid.assign(p.ssid.begin(), p.ssid.end());
    this->pass.assign(p.pass.begin(), p.pass.end());
    this->timeout = p.timeout;
    // this->timeout.assign(p.timeout.begin(), p.timeout.end());
    return true;
}

void network::module::wifi::Client::start(Params& p) {
    this->params.assign(p);
    //V2
    startV2();    
}

void network::module::wifi::Client::loop() {
    //V1
    // this->multiCheckOrReset2();

    //V2
    loopV2();

}

void network::module::wifi::Client::multiCheckOrReset() {
//     if (!this->params.apMode && !this->params.smc.smcIng && WiFi.status() != WL_CONNECTED ) {
//         GDebuger.println("WiFi connecting......");
//         pinMode(LED_BUILTIN, OUTPUT);
//         digitalWrite(LED_BUILTIN, HIGH);
//         WiFi.setAutoConnect(true);
// #ifdef ESP8266     
//         WiFi.setPhyMode(WIFI_PHY_MODE_11B);
// #endif        
//         delay(1000);
        
//         for (size_t i = 0; i < this->params.ssid.size(); i++)
//         {
//             std::string ssid = this->params.ssid[i];
//             std::string pass = this->params.pass[i];
//             int         timeout = this->params.timeout[i];
//             if (connectWifi(ssid, pass, timeout )) {
//                 break;
//             }
//         }

//         if (WiFi.status() != WL_CONNECTED) {
//             GDebuger.println("WiFi connect time out. ESP rest!\n");
//             delay(1000);
//             rfir::util::Util::Reset();                
//             return;           
//         }
        
 
//         GDebuger.println("");
//         GDebuger.println("WiFi connected\n");
        
//         GDebuger.println("========= WIFI CONNECT OK! Led Light! ==============\n");
//         IPAddress ip = WiFi.localIP();
//         String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
//         GDebuger.println("IP Address:" + ipStr);
//         digitalWrite(LED_BUILTIN, HIGH);

//     }
    
}

bool network::module::wifi::Client::connectWifi(std::string ssid, std::string pass, int timeout) {
    // GDebuger.printf("WiFi connecting......: %s %s %d \r\n", ssid.c_str(), pass.c_str(), timeout);
    // WiFi.disconnect();
    // WiFi.begin(ssid.c_str(), pass.c_str());
    // long start = millis();
    // while (WiFi.status() != WL_CONNECTED)
    // {
    //     digitalWrite(LED_BUILTIN, LOW);
    //     delay(500);
    //     if (millis() - start > timeout * 1000)
    //     {               
    //         break;
    //     }
        
    //     yield();
    //     digitalWrite(LED_BUILTIN, HIGH);
    //     delay(500);
    //     GDebuger.print(".");
    // }
    return WiFi.status() == WL_CONNECTED;

}


void network::module::wifi::Client::multiCheckOrReset2() {    
//     //非热点模式和非配网模式
//     if (!this->params.apMode && !this->params.smc.smcIng) {
//         int state = connectWifi();
//         //已经连接
//         if (state == -1) {
//             return;
//         }
//         //连接成功
//         if (state == 0) { 
//             GDebuger.println("");
//             GDebuger.println("WiFi connected\n");                
//             GDebuger.println("========= WIFI CONNECT OK! Led Light! ==============\n");
//             IPAddress ip = WiFi.localIP();
//             String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
//             GDebuger.println("IP Address:" + ipStr);   
//             return;             
//         }

//         //正在连接
//         if (state == 1) { 
//             return;
//         }

//         //连接失败   
//         if (state == 2) { 
// #if (!defined(WIFI_RESET_DISABLED) || WIFI_RESET_DISABLED == TRUE) 
//             if (WiFi.status() != WL_CONNECTED) {
//                 GDebuger.println("WiFi connect time out. ESP rest!\n");
//                 GDebuger.flush();
//                 rfir::util::Util::Reset();                
//                 return;           
//             }                    
// #endif
//         }   

//         //未配网
//         if (state == 3) {

//         }               
        
//     }    
}


int  network::module::wifi::Client::connectWifi() {
//     static int ssid_index = 0;
//     static std::string ssid_ssid;
//     static std::string ssid_pass;
//     static int         ssid_timeout = 0; 
//     static bool wifi_inited = false;
//     static bool connecting = false;
//     static unsigned long connect_start_time = 0;
//     static bool led_status = HIGH;
//     static int  led_breathe_interval = 500;
//     static unsigned long led_breathe_lasttime = 0;

//     if (WiFi.status() == WL_CONNECTED) {
//         if (wifi_inited) {
//             connecting = false;
//             connect_start_time = 0;  
//             wifi_inited = false;        
//             return 0;
//         } else {
//             return -1; //已经连接
//         }
        
//     }

//     //未配网
//     if (this->params.ssid.size() == 0) {
//         return 3;
//     }

//     //初始化
//     if (!wifi_inited) {
//         pinMode(LED_BUILTIN, OUTPUT);
//         digitalWrite(LED_BUILTIN, HIGH);        
//         WiFi.setAutoConnect(true);
// #ifdef ESP8266     
//         WiFi.setPhyMode(WIFI_PHY_MODE_11B);
// #endif 
//         WiFi.disconnect();
//         wifi_inited = true;
//     }

    
//     //是否正在连接
//     if (!connecting) { //否
//         //当前是否有连接？
//         if (ssid_index < this->params.ssid.size()) { //是    
//             ssid_ssid = this->params.ssid[ssid_index];
//             ssid_pass = this->params.pass[ssid_index];
//             ssid_timeout = this->params.timeout[ssid_index];
//             GDebuger.printf("WiFi connecting......: %s %s %d \r\n", ssid_ssid.c_str(), ssid_pass.c_str(), ssid_timeout);
//             WiFi.begin(ssid_ssid.c_str(), ssid_pass.c_str());
//             connecting = true;
//             connect_start_time = millis();
//         } else {
//             digitalWrite(LED_BUILTIN, HIGH);
//             return 2; //失败
//         }
//     }

//     //是否正在连接
//     if(connecting) { //是
//         //连接是否成功？
//         if (WiFi.status() == WL_CONNECTED) { //是
//             digitalWrite(LED_BUILTIN, LOW);
//             return 0; //成功
//         } else {
//             //连接是否超时
//             if (millis() - connect_start_time > ssid_timeout * 1000) {//是                                
//                 //是否有下一个连接？
//                 if (ssid_index + 1 < this->params.ssid.size()) { //是
//                     ssid_index++;
//                     connecting = false;
//                     connect_start_time = 0;
//                     return 1;
//                 } else { //否
//                     ssid_index = 0;
//                     connecting = false;
//                     connect_start_time = 0;  
//                     wifi_inited = false;
//                     digitalWrite(LED_BUILTIN, HIGH);
//                     return 2; //失败
//                 }

//             } else { //否
//                 //定时闪灯
//                 if (millis() - led_breathe_lasttime > led_breathe_interval) {
//                     led_status = !led_status;
//                     digitalWrite(LED_BUILTIN, led_status);  
//                     led_breathe_lasttime = millis();
//                     GDebuger.print("."); 
//                 }

//                 return 1;

//             }
//         }
//     }

    return 1; 

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

// #ifdef ESP8266     
//         WiFi.setPhyMode(WIFI_PHY_MODE_11B);
// #endif 
    if (m_connect_timeout_handler == 0) {        
        m_connect_timeout_handler = GEventTimer.delay(params.timeout, std::bind(&Client::onWifiConnectTimeout, this, std::placeholders::_1, std::placeholders::_2));
    }

    if (GLed.idle()) GLed.start(&(WIFI_CONNECT_JLED), this);
   
    if (m_connect_ssid_index < params.ssid.size()) {
        std::string ssid_ssid = this->params.ssid[m_connect_ssid_index];
        std::string ssid_pass = this->params.pass[m_connect_ssid_index];
        GDebuger.printf("Wifi connecting... %s %s \r\n", ssid_ssid.c_str(), ssid_pass.c_str());
        WiFi.begin(ssid_ssid.c_str(), ssid_pass.c_str());        
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
    GDebuger.printf("Wifi connected to : %s \r\n", WiFi.SSID().c_str());
    removeConnectTimeoutHandler();
    removeDelayConnectHandler();

    onWifiCheckTimeout(0, 0);
    GLed.stop(this);        
    return 0;
};
void* network::module::wifi::Client::onWifiDisconnect(void* arg, void* p) {
    GDebuger.printf("Wifi disconnect from : %s \r\n", WiFi.SSID().c_str());
    if (m_delay_connect_handler == 0) {
        // m_connect_ssid_index++;
        // if (m_connect_ssid_index >= params.ssid.size()) {
        //     m_connect_ssid_index = 0;
        // }
        // GLed.stop();
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
