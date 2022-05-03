#include "async-client.h"

void network::module::mqtt::AClient::init(Params p){
    this->params = p;
    WiFi.onStationModeGotIP(std::bind(&AClient::onWifiConnect, this, std::placeholders::_1));
    WiFi.onStationModeDisconnected(std::bind(&AClient::onWifiDisconnect, this, std::placeholders::_1));

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

void network::module::mqtt::AClient::start(){

};
void network::module::mqtt::AClient::loop(){};
void network::module::mqtt::AClient::connect(){};
// bool network::module::mqtt::AClient::publish(const char* msg){};
// bool network::module::mqtt::AClient::publish(const char* topic, const char* msg){};




void network::module::mqtt::AClient::connectToMqtt() {
    Serial.println("Connecting to MQTT...");
    mqtt.connect();
}

void network::module::mqtt::AClient::onWifiConnect(const WiFiEventStationModeGotIP& event) {
    Serial.println("Connected to Wi-Fi.");
    connectToMqtt();
}

void network::module::mqtt::AClient::onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
    Serial.println("Disconnected from Wi-Fi.");

}

void network::module::mqtt::AClient::onMqttConnect(bool sessionPresent) {
    Serial.println("Connected to MQTT.");
    events.onMqttConnect.emit((void*)(int)sessionPresent);
}

void network::module::mqtt::AClient::onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
    Serial.println("Disconnected from MQTT.");
    events.onMqttDisconnect.emit((void*)(int)reason);
}

void network::module::mqtt::AClient::onMqttSubscribe(uint16_t packetId, uint8_t qos) {    
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);

    events.onMqttSubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttUnsubscribe(uint16_t packetId) {
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);

    events.onMqttUnsubscribe.emit((void*)(int)packetId);
}

void network::module::mqtt::AClient::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
    Serial.println("Publish received.");
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);

    Message msg;
    msg.client = (void*)this;
    msg.topic = topic;
    msg.payload = payload;
    msg.props = &properties;
    msg.len = len;
    msg.index = index;
    msg.total = total;
    events.onMqttMessage.emit((void*)&msg);
}

void network::module::mqtt::AClient::onMqttPublish(uint16_t packetId) {
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    events.onMqttPublish.emit((void*)packetId);
}
