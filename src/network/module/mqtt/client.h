
#ifndef __NETWORK_MODULE_MQTT_CLIENT_H__
#define __NETWORK_MODULE_MQTT_CLIENT_H__

#include "rfir/util/platform.h"
#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include "MQTT.h"

namespace network {
    namespace module {
        namespace mqtt {
            class Client {
            public:
                class MQTT: public MQTTClient {
                private:
                    void* arg = 0;
                    
                public:
                    explicit MQTT(int bufSize = 3 * 1024): MQTTClient(bufSize) {};
                    void* getArg() {return arg;};
                    void  setArg(void* v) {this->arg = v;};
                };

                typedef std::function<void(MQTT* mqtt)> OnConnectEvent;
                typedef MQTTClientCallbackAdvancedFunction OnMessageEvent;
            
            public:
                struct Params {
                    std::string ip;
                    int         port = 1883;
                    int         keepalive = 10;
                    std::string user;
                    std::string pass;
                    std::string pub_topic;
                    std::string sub_topic;
                    int         bufsize = 3 * 1024;
                    std::string id;
                };

                OnMessageEvent onMessage = 0;
                OnConnectEvent onConnect = 0;

            private:
                WiFiClient* net = 0;
                MQTT* mqtt = 0;
                Params params;
            public:

                Client();
                ~Client();
                void init(Params p);
                void uninit();

                void start();
                void loop();
                void connect();
                bool publish(const char* msg);
            public:
                static ICACHE_RAM_ATTR void OnMessage(MQTTClient *client, char topic[], char bytes[], int length);
 
            };

        }
    }
}



#endif //__NETWORK_SERVICE_MQTT_CLIENT_H__
