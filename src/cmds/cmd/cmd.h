#ifndef CMDS_CMD_CMD_H
#define CMDS_CMD_CMD_H

#include "rfir/util/event-emitter.h"
#include "rfir/util/reg-table.h"

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
            static const uint8_t resetconfig = 11;
            static const uint8_t device_joined = 12;
            static const uint8_t device_leave = 13;
            static const uint8_t device_interview = 14;
        };

        class Cmd {     
        public:
            static const uint16_t PRO_LOGO = 0x4E44;
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
                uint16_t    pro_logo = 0;        
                uint8_t     pro_ver = 0;
                uint8_t     dev_id = 0;
                uint8_t     cmd_id = 0;
                uint8_t     cmd_stp = 0;
                uint16_t    err_no = 0;
                uint32_t    cmd_sid = 0;                
                uint16_t    pld_sum = 0;
                uint16_t    pld_len = 0;
            };
            
        public: 
            Events events;
            Params params;      
            Head*  head;   
            char*  payload;  
            rfir::util::RegTable regTable;

        public:
            static  void reset(Head* head);  
            virtual bool send(std::list<int> ids){ return 0;};
            virtual bool recv(const char* buf, int size) {return 0;};        
            virtual void reset(){};  
            virtual bool encode(){return 0;};
            virtual bool decode(){return 0;};
        };

        class RecvCmd: public Cmd {
        public:
            bool recv(const char* buf, int size) override;  
            bool decode() override;
            void reset() override;  
        };

        class SendCmd: public Cmd {
         public:
            bool send(std::list<int> ids) override;   
            bool send(); 
            bool encode() override;
            void reset() override;          
        };
    }
}
                
extern cmds::cmd::RecvCmd GRecvCmd;
extern cmds::cmd::SendCmd GSendCmd;

#endif                