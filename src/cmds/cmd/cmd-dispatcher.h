

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
            CmdDispatcher(cmds::network::Signaler* p);
            ~CmdDispatcher();
        public:
            Events events;
            rfir::util::MapFactory<uint32_t, cmds::cmd::CmdBase* > send_queue;
            bool sendCmd(cmds::cmd::CmdBase* cmd);
        public:
            static void* OnConnect(void* arg, void* p);
            static void* OnDisconnect(void* arg, void* p);
            static void* OnMessage(void* arg, void* p);       

        public:
            static void* OnResp(void* arg, void* p);
            static void* OnRespTimeout(void* arg, void* p);
        };

        class MqttDispatcher: public CmdDispatcher {
        public:
            MqttDispatcher(cmds::network::MqttSignaler* p);
            ~MqttDispatcher();
        public:
            static void* OnMqttMessage(void* arg, void* p);       
        };        
    }
}

extern cmds::cmd::CmdDispatcher* GCmdDispatcher;

#endif 
