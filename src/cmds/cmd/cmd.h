#ifndef CMDS_CMD_CMD_H
#define CMDS_CMD_CMD_H

#include "rfir/util/event-emitter.h"

namespace cmds {
    namespace cmd {
        struct CmdId {
            static const uint8_t config = 1;
            static const uint8_t handshake = 2;
            static const uint8_t reboot = 3;
            static const uint8_t get = 4;
            static const uint8_t set = 5;
            static const uint8_t report = 6;
            static const uint8_t penet = 7;
            static const uint8_t update = 8;
            static const uint8_t online = 9;
            static const uint8_t offline = 10;
        };

        class Cmd {     
        public:
            struct Events {
                rfir::util::Event recv;
                rfir::util::Event send;
            };

            struct Params {
                char* buf = 0;
                int   bufsize = 0;
            };   

            struct  Head
            {
                uint8_t     pro_ver = 0;
                uint8_t     dev_id = 0;
                uint8_t     cmd_id = 0;
                uint8_t     cmd_stp = 0;
                uint32_t    cmd_sid = 0;
                uint8_t     err_no = 0;
                uint8_t     con_sum = 0;
                uint16_t    con_len = 0;
            };
            
        public: 
            Events events;
            Params params;      
            Head*  head;     
        public:
            bool send(std::list<int> ids);
            bool recv(const char* buf, int size);        
            void reset();  
        };
    }
}
                
extern cmds::cmd::Cmd GCmdRecv;
extern cmds::cmd::Cmd GCmdSend;

#endif                