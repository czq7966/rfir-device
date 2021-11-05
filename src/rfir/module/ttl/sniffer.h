//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_GPIO_TTL_SNIFFER_H__
#define __RFIR_MODULE_GPIO_TTL_SNIFFER_H__

#include "rfir/util/platform.h"
#include "gpio.h"

namespace rfir {
    namespace module {
        namespace ttl {
            struct Data
            {
                bool ttl; 
                timeval time;
            };

            struct Delta
            {
                uint16_t delta;
            };            

            class RFIR;
            class Sniffer {
            public:
                Delta* sniffedDelta = 0;
                Data*  dataBuf = 0;
                Data   preTTL = {};
                unsigned long preMicros = 0;

                uint32_t pushCount = 0;
                uint32_t popCount = 0;
                uint32_t sniffedCount = 0;
                bool     sniffStopped = false;            

            public:
                typedef struct {
                    bool success;   // Was the match successful?
                    uint64_t data;  // The data found.
                    uint16_t used;  // How many buffer positions were used.
                } match_result_t;   

                typedef std::function<void(Sniffer* sniffer, Delta* data, int count)> OnSniffed;  

                struct Params {
                    uint8_t     pin = 0;
                    uint8_t     mode = INPUT_PULLUP;
                    bool        inverted = false;
                    uint16_t    minCount = 50;
                    uint32_t    minDelta = 150;
                    uint32_t    maxDelta = 65535;
                    uint32_t    bufSize = 256;
                    std::string toString();
                    bool        parseFromJson(neb::CJsonObject* jp);  
                };

                struct SniffParams {
                    Params params;
                    bool   response = 0;
                    std::string toString();
                    bool   parseFromJson(neb::CJsonObject* jp); 
                    bool   clone(SniffParams* p);
                };

            private:
                SniffParams sniffParams;
            public:
                std::string name;
                RFIR*       rfir = 0;                
                Gpio        gpio;
                OnSniffed   onSniffed = 0;

            public:
                Sniffer(RFIR* rfir = 0);
                Sniffer(RFIR* rfir, SniffParams params);
                ~Sniffer();

                void init(SniffParams params);
                void uninit();                
                void start();
                void stop();
                bool started();
                SniffParams* getSniffParams();

                
                uint32_t pushTTL(bool ttl);
                uint32_t popTTL(Data* ttl);
                bool     getTTL(uint32_t idx , Data* ttl);
                bool     getTTLDelta(Data* ttl);
                bool     getTTLDelta(Data* first, Data* second, Data* ttl);

                int      getTTLElapse(Data& ttl, Data* data);

                int      sniffData();
                int      sniffDelta();
                void     resumeSniff();
                void     stopSniff();
                void     resetSniff();
                int      getBufSize();
                Delta*   getSniffedDelta();
                int      getSniffedCount();
                String   toString();

            public:
                static void       onGpioChange(Gpio* gpio, int value);
                static std::string packSniffedCmd(Sniffer* sniffer, const char* data);
            };
        }
    }
}


#endif //__RFIR_MODULE_GPIO_TTL_SNIFFER_H__
