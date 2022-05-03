
#ifndef __NETWORK_MODULE_MQTT_ACLIENT_H__
#define __NETWORK_MODULE_MQTT_ACLIENT_H__

#include "rfir/util/platform.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include "AsyncMqttClient.h"

#include "rfir/util/event-emitter.h"

namespace network {
    namespace module {
        namespace mqtt {
            class AClient {
           
            public:
                struct Params {
                    std::string ip;
                    int         port = 1883;
                    int         keepalive = 15;
                    std::string user;
                    std::string pass;
                    std::string id;
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
                AsyncMqttClient mqtt;
                Params params;
            public:

                void init(Params p);
                void uninit();

                void start();
                void loop();
                void connect();
                bool publish(const char* msg);
                bool publish(const char* topic, const char* msg);
            public:
                void connectToMqtt();
                void onWifiConnect(const WiFiEventStationModeGotIP& event);
                void onWifiDisconnect(const WiFiEventStationModeDisconnected& event);
                void onMqttConnect(bool sessionPresent);
                void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
                void onMqttSubscribe(uint16_t packetId, uint8_t qos);
                void onMqttUnsubscribe(uint16_t packetId);
                void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
                void onMqttPublish(uint16_t packetId);
            };

        }
    }
}



#endif //
