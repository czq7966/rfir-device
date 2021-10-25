#include "client.h"
#include "rfir/util/util.h"

network::module::mqtt::Client::Client() {
    
}

network::module::mqtt::Client::~Client() {
    uninit();
}

void network::module::mqtt::Client::init(Params p) {
    this->params = p;
    this->net = new WiFiClient();
    this->mqtt = new MQTT(p.bufsize);
    this->mqtt->setArg(this);    
}

void network::module::mqtt::Client::uninit() {
    this->mqtt->setArg(0);   
    this->mqtt->disconnect(); 
    delete this->mqtt;    
    delete this->net;
    this->mqtt = 0;
    this->net = 0;
}


void network::module::mqtt::Client::start() {
    mqtt->begin(this->params.ip.c_str(), this->params.port,  *net);
    mqtt->onMessageAdvanced(OnMessage); 
}

void network::module::mqtt::Client::loop() {
    mqtt->loop();
    
    if (!mqtt->connected()) 
        connect();  
}

void network::module::mqtt::Client::connect() {
    if(WiFi.status() == WL_CONNECTED) {
        Serial.println("mqtt connecting...:" + String(this->params.ip.c_str()) + ":" + String(this->params.port));
        digitalWrite(LED_BUILTIN, LOW);        
        
        if (!mqtt->connect(this->params.id.c_str() , this->params.user.c_str(), this->params.pass.c_str()) && (WiFi.status() == WL_CONNECTED)) {
            delay(300);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(700);
        }

        if (mqtt->connected()) {

            Serial.println("\nmqtt connected!");
            digitalWrite(LED_BUILTIN, HIGH);

            mqtt->subscribe(this->params.sub_topic.c_str()); 
            if (this->onConnect) {
                this->onConnect(mqtt);
            }
        }        
    }
}


void network::module::mqtt::Client::OnMessage(MQTTClient *mqttClient, char topic[], char bytes[], int length) {
    auto mqtt = (MQTT*)mqttClient;
    auto client = (Client*)(mqtt->getArg());
    if (client->onMessage)
        client->onMessage(mqtt, topic, bytes, length);
}


bool network::module::mqtt::Client::publish(const char* msg) {    
    if (mqtt->connected())
        return mqtt->publish(this->params.pub_topic.c_str(), msg);
    return 0;
}
