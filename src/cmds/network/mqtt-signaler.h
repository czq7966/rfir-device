#ifndef __CMDS_NETWORK_MQTT_SIGNALER_H
#define __CMDS_NETWORK_MQTT_SIGNALER_H

#include "signaler.h"
#include "network/module/mqtt/client.h"
#include "../cmd/cmd-mqtt.h"

namespace cmds {
    namespace network {
        class MqttSignaler: public Signaler {
        protected:      
            ::network::module::mqtt::Client* mqtt = 0;
        public:
            MqttSignaler(::network::module::mqtt::Client* p);
            void setMqtt(::network::module::mqtt::Client* p);
        public:
            virtual int write(void* p) override;
            virtual int write(cmds::cmd::CmdMqtt* p);
        };
    }
}

extern cmds::network::MqttSignaler GMqttSignaler;

#endif //__CMDS_NETWORK_SIGNALER_H
