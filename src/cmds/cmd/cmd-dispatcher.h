

#ifndef __CMDS_CMD_DISPATCHER_H
#define __CMDS_CMD_DISPATCHER_H

#include "cmd-base.h"
#include "cmd-mqtt.h"
#include "rfir/util/map-factory.h"
#include "../network/signaler.h"
#include "../network/mqtt-signaler.h"

namespace cmds {
    namespace cmd {
        class CmdDispatcher {
        public:
            struct Events {
                rfir::util::Event onConnect;        
                rfir::util::Event onDisconnect;     
                rfir::util::Event onMessage;    
                rfir::util::Event onCommand;
            };        

        public:
            cmds::network::Signaler* signaler;
            virtual void setSignaler(cmds::network::Signaler* p);
        public:
            Events events;
            rfir::util::MapFactory<uint32_t, cmds::cmd::CmdBase::Events > wait_resp_queue;
            bool sendCmd(cmds::cmd::CmdBase* cmd);
        public:
            void* onConnect(void* arg, void* p);
            void* onDisconnect(void* arg, void* p);
            void* onMessage(void* arg, void* p);       

        public:
            void* onResp(void* arg, void* p);
            void* onRespTimeout(void* arg, void* p);
        };

        class MqttDispatcher: public CmdDispatcher {
        public:
            void* onMqttMessage(void* arg, void* p);   
            void setSignaler(cmds::network::Signaler* p) override;    
        };        
    }
}

extern cmds::cmd::MqttDispatcher GCmdDispatcher;

#endif 
