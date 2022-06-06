#include "async-client.h"
#include "rfir/util/event-timer.h"
#include "rfir/util/util.h"
#include "rfir/util/led.h"
#include "../wifi/client.h"

void network::module::mqtt::AClient::init(Params p){
    DEBUGER.println(" network::module::mqtt::AClient::init");
    this->params = p;
    // wifiConnectHandler = WiFi.onStationModeGotIP(std::bind(&AClient::onWifiConnect, this, std::placeholders::_1));
    // wifiDisconnectHandler = WiFi.onStationModeDisconnected(std::bind(&AClient::onWifiDisconnect, this, std::placeholders::_1));
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
    mqtt.setServer(params.ip.c_str(), params.port);
    mqtt.setClientId(params.id.c_str());
    mqtt.setKeepAlive(params.keepalive);
    mqtt.setCredentials(params.user.c_str(), params.pass.c_str());
};
void network::module::mqtt::AClient::uninit(){};

void network::module::mqtt::AClient::start(Params p){
    init(p);
};
void network::module::mqtt::AClient::loop(){

};

void network::module::mqtt::AClient::setWill(const char* topic, const char* payload, bool retain, uint8_t qos, size_t length){
    params.willTopic = topic;
    params.willPayload = payload;
    mqtt.setWill(params.willTopic.c_str(), qos, retain, params.willPayload.c_str());
};
uint16_t network::module::mqtt::AClient::publish(const char* topic, const char* payload, bool retain, uint8_t qos, size_t length, bool dup, uint16_t message_id){
    return mqtt.publish(topic, qos, retain, payload, length, dup, message_id);
};



void network::module::mqtt::AClient::connectToMqtt() {
    if (WiFi.isConnected() && !mqtt.connected()) {
        DEBUGER.println("Connecting to MQTT...");
        if (m_connect_timeout_handler == 0) {        
            m_connect_timeout_handler = GEventTimer.delay(params.timeout, std::bind(&AClient::onConnectToMqttTimeout, this, std::placeholders::_1, std::placeholders::_2));
        }
        if (GLed.idle())
            GLed.start(&(MQTT_CONNECT_JLED));

        mqtt.connect();
    }
}

void network::module::mqtt::AClient::disconnectToMqtt(bool force) {
    if (mqtt.connected()) {
        DEBUGER.println("Disconnecting to MQTT...");
        mqtt.disconnect(force);
    }
}


void* network::module::mqtt::AClient::onWifiConnect(void* arg, void* p) {
    DEBUGER.println("Connected to Wi-Fi.");
    delayConnectToMqtt();
    return 0;
}

void* network::module::mqtt::AClient::onWifiDisconnect(void* arg, void* p) {
    DEBUGER.println("Disconnected from Wi-Fi.");
    return 0;
}

void network::module::mqtt::AClient::onMqttConnect(bool sessionPresent) {
    DEBUGER.println("Connected to MQTT.");
    GEventTimer.remove(m_connect_timeout_handler);
    m_connect_timeout_handler = 0;
    GLed.stop();
    events.onMqttConnect.emit((void*)(int)sessionPresent);
}

void network::module::mqtt::AClient::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    DEBUGER.printf("Disconnected from MQTT. %d \r\n", reason);
    events.onMqttDisconnect.emit((void*)(int)reason);
    GLed.stop();
    if (WiFi.isConnected())
        GEventTimer.delay(1000, std::bind(&AClient::doConnectToMqtt, this, std::placeholders::_1, std::placeholders::_2) );

}

void network::module::mqtt::AClient::onMqttSubscribe(uint16_t packetId, uint8_t qos) {    
    // DEBUGER.println("Subscribe acknowledged.");
    // DEBUGER.print("  packetId: ");
    // DEBUGER.println(packetId);
    // DEBUGER.print("  qos: ");
    // DEBUGER.println(qos);

    events.onMqttSubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttUnsubscribe(uint16_t packetId) {
    // DEBUGER.println("Unsubscribe acknowledged.");
    // DEBUGER.print("  packetId: ");
    // DEBUGER.println(packetId);

    events.onMqttUnsubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    // DEBUGER.println("Publish received.");
    // DEBUGER.print("  topic: ");
    // DEBUGER.println(topic);
    // DEBUGER.print("  qos: ");
    // DEBUGER.println(properties.qos);
    // DEBUGER.print("  dup: ");
    // DEBUGER.println(properties.dup);
    // DEBUGER.print("  retain: ");
    // DEBUGER.println(properties.retain);
    // DEBUGER.print("  len: ");
    // DEBUGER.println(len);
    // DEBUGER.print("  index: ");
    // DEBUGER.println(index);
    // DEBUGER.print("  total: ");
    // DEBUGER.println(total);
    if (total > MsgBufSize) {
        DEBUGER.printf("Mqtt message max length %d, min length 1. actual total: %d\r\n", MsgBufSize, total);
        return ;
    }

    memcpy(msgBuf + index, payload, len);
    if (index + len == total) {
        Message msg;
        msg.client = (void*)this;
        msg.topic = topic;
        msg.payload = msgBuf;
        msg.props = &properties;
        msg.len = len;
        msg.index = index;
        msg.total = total;
        events.onMqttMessage.emit((void*)&msg);
        memset(msgBuf, 0, sizeof(msgBuf));
    }
}

void network::module::mqtt::AClient::onMqttPublish(uint16_t packetId) {
    // DEBUGER.println("Publish acknowledged.");
    // DEBUGER.print("  packetId: ");
    // DEBUGER.println(packetId);
    events.onMqttPublish.emit((void*)packetId);
}

void network::module::mqtt::AClient::delayConnectToMqtt(int timeout_ms){
    GEventTimer.delay(timeout_ms, std::bind(&AClient::doConnectToMqtt, this, std::placeholders::_1, std::placeholders::_2));  
};

void network::module::mqtt::AClient::delayDisconnectToMqtt(int timeout_ms) {
    GEventTimer.delay(timeout_ms, std::bind(&AClient::doDisconnectToMqtt, this, std::placeholders::_1, std::placeholders::_2));
};

void*  network::module::mqtt::AClient::doConnectToMqtt(void* arg, void* p) {
    this->connectToMqtt();
    return 0;
}

void*  network::module::mqtt::AClient::doDisconnectToMqtt(void* arg, void* p) {
    this->disconnectToMqtt(true);
    return 0;
}

void* network::module::mqtt::AClient::onConnectToMqttTimeout(void* arg, void* p){
    rfir::util::Util::Reset();
    return 0;
};

network::module::mqtt::AClient GMqttClient;