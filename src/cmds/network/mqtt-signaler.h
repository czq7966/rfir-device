#ifndef __CMDS_NETWORK_MQTT_SIGNALER_H
#define __CMDS_NETWORK_MQTT_SIGNALER_H

#include "signaler.h"
#include "network/module/mqtt/async-client.h"
#include "../cmd/cmd-mqtt.h"

namespace cmds {
    namespace network {
        class MqttSignaler: public Signaler {
        public:      
            ::network::module::mqtt::AClient* mqtt = 0;
        public:

            void setMqtt(::network::module::mqtt::AClient* p);
        public:
            virtual int write(void* p) override;
            virtual int write(cmds::cmd::CmdMqtt* p);
        };
    }
}

extern cmds::network::MqttSignaler GMqttSignaler;

#endif //__CMDS_NETWORK_SIGNALER_H
