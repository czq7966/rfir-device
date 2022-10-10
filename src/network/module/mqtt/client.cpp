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
    mqtt->setKeepAlive(this->params.keepalive);
    mqtt->begin(this->params.ip.c_str(), this->params.port,  *net);
    mqtt->onMessageAdvanced(OnMessage); 
}

void network::module::mqtt::Client::loop() {
    mqtt->loop();
    
    if (!mqtt->connected()) 
        connect();  
}

void network::module::mqtt::Client::connect() {
    static unsigned long Mqtt_client_connect_time = 0;

    if(WiFi.status() == WL_CONNECTED) {
        GDebuger.println("mqtt connecting...:" + String(this->params.ip.c_str()) + ":" + String(this->params.port));
        digitalWrite(LED_BUILTIN, LOW);        
        
        if (!mqtt->connect(this->params.id.c_str() , this->params.user.c_str(), this->params.pass.c_str()) && (WiFi.status() == WL_CONNECTED)) {
            delay(300);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(700);
        }

        if (Mqtt_client_connect_time == 0)
            Mqtt_client_connect_time = millis();

        if (mqtt->connected()) {
            Mqtt_client_connect_time = 0;
            GDebuger.println("\nmqtt connected!");
            digitalWrite(LED_BUILTIN, HIGH);

            mqtt->subscribe(this->params.sub_topic.c_str()); 
            if (this->onConnect) {
                this->onConnect(mqtt);
            }
            this->events.onConnect.emit((void*)this);
        } else { //超时重启
            if (millis() - Mqtt_client_connect_time > Config.props.mqtt_reset_timeout)  {
                GDebuger.println("MQTT connect time out. ESP reset!\n");
                delay(1000);
                rfir::util::Util::Reset();
            }
        }        
    }
}


void network::module::mqtt::Client::OnMessage(MQTTClient *mqttClient, char topic[], char bytes[], int length) {
    auto mqtt = (MQTT*)mqttClient;
    auto client = (Client*)(mqtt->getArg());
    if (client->onMessage)
        client->onMessage(mqtt, topic, bytes, length);

    Message msg;
    msg.client = client;
    msg.topic = topic;
    msg.bytes = bytes;
    msg.length = length;
    client->events.onMessage.emit((void*)&msg);

}


bool network::module::mqtt::Client::publish(const char* msg) {    
    if (mqtt->connected())
        return mqtt->publish(this->params.pub_topic.c_str(), msg);
    return 0;
}

bool network::module::mqtt::Client::publish(const char* topic, const char* msg) {
    GDebuger.println("network::module::mqtt::Client::publish:");    
    if (mqtt->connected()) {
        GDebuger.println(topic);
        GDebuger.println(msg);
        return mqtt->publish(topic, msg);
    }
    return 0;
}