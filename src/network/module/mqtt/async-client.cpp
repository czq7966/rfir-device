#include "async-client.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "rfir/util/debuger.h"
#include "../wifi/client.h"

void network::module::mqtt::AClient::init(){
    
    GWifiClient.events.onWifiConnect.add(this, std::bind(&AClient::onWifiConnect, this, std::placeholders::_1, std::placeholders::_2));
    GWifiClient.events.onWifiDisconnect.add(this, std::bind(&AClient::onWifiDisconnect, this, std::placeholders::_1, std::placeholders::_2));

    mqtt.onConnect(std::bind(&AClient::onMqttConnect, this, std::placeholders::_1));
    mqtt.onDisconnect(std::bind(&AClient::onMqttDisconnect, this, std::placeholders::_1));
    mqtt.onSubscribe(std::bind(&AClient::onMqttSubscribe, this, std::placeholders::_1, std::placeholders::_2));
    mqtt.onUnsubscribe(std::bind(&AClient::onMqttUnsubscribe, this, std::placeholders::_1));
    mqtt.onMessage(std::bind(&AClient::onMqttMessage, this 
                        , std::placeholders::_1, std::placeholders::_2
                        , std::placeholders::_3, std::placeholders::_4
                        , std::placeholders::_5, std::placeholders::_6));
    mqtt.onPublish(std::bind(&AClient::onMqttPublish, this, std::placeholders::_1));
    mqtt.setServer(params.ip, params.port);
    mqtt.setClientId(params.id);
    mqtt.setKeepAlive(params.keepalive);
    mqtt.setCleanSession(params.clean_session);
    mqtt.setCredentials(params.user, params.pass);
};
void network::module::mqtt::AClient::uninit(){

};

void network::module::mqtt::AClient::start(){
    init();
};
void network::module::mqtt::AClient::loop(){

};


uint16_t network::module::mqtt::AClient::publish(const char* topic, const char* payload, bool retain, uint8_t qos, size_t length, bool dup, uint16_t message_id){
    return mqtt.publish(topic, qos, retain, payload, length, dup, message_id);
};



void network::module::mqtt::AClient::connectToMqtt() {
    if (WiFi.isConnected()) {
        if (this->params.enable && !mqtt.connected() && strcmp(this->params.ip, "") != 0) {
            GDebuger.print(F("Connecting to MQTT... "));
            GDebuger.println(this->params.ip);
            if (m_connect_timeout_handler == 0) {        
                m_connect_timeout_handler = GEventTimer.delay(params.timeout, std::bind(&AClient::onConnectToMqttTimeout, this, std::placeholders::_1, std::placeholders::_2));
            }
            if (GLed.idle() && this->params.jled)
                GLed.start(this->params.jled, this);

            mqtt.connect();
        }
    }
}

void network::module::mqtt::AClient::disconnectToMqtt(bool force) {
    if (mqtt.connected()) {
        GDebuger.print(F("Disconnecting from MQTT... "));
        GDebuger.println(this->params.ip);
        mqtt.disconnect(force);
    }
}


void* network::module::mqtt::AClient::onWifiConnect(void* arg, void* p) {
    delayConnectToMqtt();
    return 0;
}

void* network::module::mqtt::AClient::onWifiDisconnect(void* arg, void* p) {
    mqtt.disconnect(true);
    return 0;
}

void network::module::mqtt::AClient::onMqttConnect(bool sessionPresent) {
    GDebuger.print(F("Connected to MQTT. "));
    GDebuger.println(this->params.ip);
    GEventTimer.remove(m_connect_timeout_handler);
    m_connect_timeout_handler = 0;
    onCheckToMqttTimeout(0, 0);
    GLed.stop(this);
    events.onMqttConnect.emit((void*)(int)sessionPresent);
}

void network::module::mqtt::AClient::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    GDebuger.print(F("Disconnected from MQTT. "));
    GDebuger.println(this->params.ip);
    events.onMqttDisconnect.emit((void*)(int)reason);
    GLed.stop(this);
    if (WiFi.isConnected())
        GEventTimer.delay(1000, std::bind(&AClient::doConnectToMqtt, this, std::placeholders::_1, std::placeholders::_2) );

}

void network::module::mqtt::AClient::onMqttSubscribe(uint16_t packetId, uint8_t qos) {    
    events.onMqttSubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttUnsubscribe(uint16_t packetId) {
    events.onMqttUnsubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    if (total > this->params.msgbufsize) {
        GDebuger.print(F("Mqtt message max length "));
        GDebuger.print(this->params.msgbufsize);
        GDebuger.print(F(" min length 1. actual total: "));
        GDebuger.println(total);
        return;
    }

    memcpy(this->params.msgbuf + index, payload, len);
    if (index + len == total) {
        Message msg;
        msg.client = (void*)this;
        msg.topic = topic;
        msg.payload = this->params.msgbuf;
        msg.props = &properties;
        msg.len = len;
        msg.index = index;
        msg.total = total;
        events.onMqttMessage.emit((void*)&msg);
        memset(this->params.msgbuf, 0, sizeof(this->params.msgbuf));
    }
}

void network::module::mqtt::AClient::onMqttPublish(uint16_t packetId) {
    events.onMqttPublish.emit((void*)packetId);
}

void network::module::mqtt::AClient::delayConnectToMqtt(int timeout_ms){
    if (m_delay_connect_handler == 0)
        m_delay_connect_handler = GEventTimer.delay(timeout_ms, std::bind(&AClient::doConnectToMqtt, this, std::placeholders::_1, std::placeholders::_2));  
};

void network::module::mqtt::AClient::delayDisconnectToMqtt(int timeout_ms) {
    GEventTimer.delay(timeout_ms, std::bind(&AClient::doDisconnectToMqtt, this, std::placeholders::_1, std::placeholders::_2));
};

void*  network::module::mqtt::AClient::doConnectToMqtt(void* arg, void* p) {
    m_delay_connect_handler = 0;
    this->connectToMqtt();
    return 0;
}

void*  network::module::mqtt::AClient::doDisconnectToMqtt(void* arg, void* p) {
    this->disconnectToMqtt(true);
    return 0;
}

void* network::module::mqtt::AClient::onConnectToMqttTimeout(void* arg, void* p){
    // rfir::util::Util::Reset();
    this->events.onMqttConnectTimeout.emit(this);
    return 0;
};

void* network::module::mqtt::AClient::onCheckToMqttTimeout(void* arg, void* p){
    if (!mqtt.connected() && m_connect_timeout_handler == 0) {
        connectToMqtt();
    }

    GEventTimer.remove(m_check_timeout_handler);
    m_check_timeout_handler = GEventTimer.delay(params.interval, std::bind(&AClient::onCheckToMqttTimeout, this, std::placeholders::_1, std::placeholders::_2));
    return 0;
};

network::module::mqtt::AClient GMqttClient;
network::module::mqtt::AClient LMqttClient;