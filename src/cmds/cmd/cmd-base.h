#ifndef __CMDS_CMD_CMD_BASE_H__
#define __CMDS_CMD_CMD_BASE_H__

#include <string>
#include "rfir/util/event-emitter.h"
#include "rfir/util/cjson/CJsonObject.hpp"

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

                    uint32_t sid = 0;
                    uint32_t stp = 0;
                };


               
                static Address* DefaultFrom;
                static Address* DefaultTo;
                static int      DefaultRespTimeout;
                Head head;
                neb::CJsonObject hd;
                neb::CJsonObject pld;

                Command();
                uint32_t getSid();
                bool setSid(uint32_t value);
                bool isReqCmd();
                bool isRespCmd();
                bool setReqCmd();
                bool setRespCmd();

                bool isNeedResp();
                bool setNeedResp(bool value = 1);

                void fixUp();
                void fixDown();

                void cloneFrom(Command& cmd);
                void cloneTo(Command& cmd);
                void cloneFrom(neb::CJsonObject& cmd);
                void cloneTo(neb::CJsonObject& cmd);
                void cloneFrom(neb::CJsonObject& hd, neb::CJsonObject& pld);
                void cloneTo(neb::CJsonObject& hd, neb::CJsonObject& pld);



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
