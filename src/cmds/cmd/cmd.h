#ifndef CMDS_CMD_CMD_H
#define CMDS_CMD_CMD_H

#include "rfir/util/event-emitter.h"

namespace cmds {
    namespace cmd {
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
                uint16_t pro_no = 0;
                uint16_t pro_ver = 0;
                uint16_t dev_id = 0;
                uint16_t cmd_id = 0;
                uint16_t cmd_stp = 0;
                uint16_t err_no = 0;
                uint32_t cmd_sid = 0;
                uint16_t con_len = 0;
                uint16_t con_sum = 0;          
            };
            
        public: 
            Events events;
            Params params;      
            Head*  head;     
        public:
            bool send(std::list<int> ids);
            bool recv(const char* buf, int size);          
        };
    }
}
                
extern cmds::cmd::Cmd GCmdRecv;
extern cmds::cmd::Cmd GCmdSend;

#endif                