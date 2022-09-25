#ifndef __CMDS_CMD_CMD_BASE_H__
#define __CMDS_CMD_CMD_BASE_H__

#include <string>
#include "rfir/util/event-emitter.h"
#include <ArduinoJson.h>

namespace cmds {
    namespace cmd {
        class CmdBase {
        public:
            struct Command {
                static uint32_t& SessionID;
                struct Address {                    
                    std::string type;
                    std::string id;
                };
                struct Head {                    
                    Address from;
                    Address to;
                    Address entry;

                    std::string sid = "";
                    uint32_t stp = 0;
                    uint8_t  qos = 2;
                    bool     retain = false;
                };


               
                static Address* DefaultFrom;
                static Address* DefaultTo;
                static int      DefaultRespTimeout;
                Head head;

                DynamicJsonDocument* doc;
                JsonObject root, hd, pld;

                Command();
                ~Command();
                std::string getSid();                
                uint32_t getIntSid();
                bool setSid(std::string value);                
                bool setSid(uint32_t value);
                bool resetHead();
                bool isReqCmd();
                bool isRespCmd();
                bool setReqCmd();
                bool setRespCmd();

                bool isNeedResp();
                bool setNeedResp(bool value = 1);

                void fixUp();
                void fixDown();
                void fixUpRecv();

                void cloneFrom(Command& cmd);
                void cloneTo(Command& cmd);

                void cloneFrom(JsonObject cmd);
                void cloneTo(JsonObject cmd);
                void cloneFrom(JsonObject hd, JsonObject pld);
                void cloneTo(JsonObject hd, JsonObject pld);

                std::string toString();
                void        fromString(const char* str);

            };

            struct Events {
                rfir::util::EventEmitter::Event onTimeout;   
                rfir::util::EventEmitter::Event onResp;      
                // void cloneFrom(Events& events);
                // void cloneTo(Events& events);  
                // Events* clone();           
            };

        public:
            Command command;
            Events events;

            int respTimeout = 10 * 1000;

            virtual  bool send(const void* p = 0);
            virtual  void cloneFrom(CmdBase& cmd);
            virtual  void cloneTo(CmdBase& cmd);      
        };
    }
}


#endif //__CMDS_CMD_CMD_BASE_H__
