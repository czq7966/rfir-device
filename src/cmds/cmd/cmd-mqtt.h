#ifndef __CMDS_CMD_MQTT_CMD_H__
#define __CMDS_CMD_MQTT_CMD_H__

#include "cmd-base.h"

namespace cmds {
    namespace cmd {
        class CmdMqtt: public CmdBase {
        private:
            std::string zeroTopic(std::string topic);
        public:
            std::string topic;
            std::string expandTopic();
        public:
            virtual bool send(const void* p = 0) override; 
        };
    }
}


#endif //__CMDS_CMD_CMD_BASE_H__
