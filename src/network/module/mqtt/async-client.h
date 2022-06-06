
#ifndef __NETWORK_MODULE_MQTT_ACLIENT_H__
#define __NETWORK_MODULE_MQTT_ACLIENT_H__

#include "rfir/util/platform.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include "AsyncMqttClient.h"
#include <jled.h>
#include "rfir/util/event-emitter.h"

namespace network {
    namespace module {
        namespace mqtt {
            class AClient {
            #ifdef MQTT_BUFFER_SIZE
                static const int MsgBufSize = MQTT_BUFFER_SIZE;
            #else
                static const int MsgBufSize = 2048;
            #endif            
           
            public:
                struct Params {
                    std::string ip;
                    int         port = 1883;
                    int         keepalive = 15;
                    int         timeout = 1000 * 60;
                    std::string user;
                    std::string pass;
                    std::string id;
                    std::string willTopic;
                    std::string willPayload;
                };

                struct Message {
                    void*       client;
                    char*       topic;
                    char*       payload;
                    AsyncMqttClientMessageProperties* props;
                    size_t      len;
                    size_t      index;
                    size_t      total;
                };

                class Events  {
                public:
                    rfir::util::Event onMqttConnect;
                    rfir::util::Event onMqttDisconnect;
                    rfir::util::Event onMqttSubscribe;
                    rfir::util::Event onMqttUnsubscribe;
                    rfir::util::Event onMqttMessage;
                    rfir::util::Event onMqttPublish;
                };
                
                
                Events events;
            public:
                char msgBuf[MsgBufSize];
                AsyncMqttClient mqtt;
                Params params;
                uint32_t        m_connect_timeout_handler = 0;
            public:

                void init(Params p);
                void uninit();

                void start(Params p);
                void loop();

                void setWill(const char* topic, const char* payload = nullptr, bool retain = true, uint8_t qos = 2, size_t length = 0);
                uint16_t publish(const char* topic, const char* payload = nullptr, bool retain = false, uint8_t qos = 2, size_t length = 0, bool dup = false, uint16_t message_id = 0);
  
            public:            
                void connectToMqtt();
                void disconnectToMqtt(bool force = false);
                
                void* onWifiConnect(void* arg, void* p);
                void* onWifiDisconnect(void* arg, void* p);

                void onMqttConnect(bool sessionPresent);
                void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
                void onMqttSubscribe(uint16_t packetId, uint8_t qos);
                void onMqttUnsubscribe(uint16_t packetId);
                void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
                void onMqttPublish(uint16_t packetId);
            public:
                void  delayConnectToMqtt(int timeout_ms = 100);
                void  delayDisconnectToMqtt(int timeout_ms = 1000);
                void* doConnectToMqtt(void* arg, void* p);
                void* doDisconnectToMqtt(void* arg, void* p);

                void* onConnectToMqttTimeout(void* arg, void* p);
            };

        }
    }
}

extern network::module::mqtt::AClient GMqttClient;

#endif //
