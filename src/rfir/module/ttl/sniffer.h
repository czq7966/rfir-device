//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_MODULE_GPIO_TTL_SNIFFER_H__
#define __RFIR_MODULE_GPIO_TTL_SNIFFER_H__

#include "rfir/util/platform.h"
#include "gpio.hpp"

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
                    uint8_t     pin = 22;
                    uint8_t     mode = INPUT_PULLUP;
                    bool        inverted = false;
                    uint16_t    minCount = 50;
                    uint32_t    minDelta = 150;
                    uint32_t    maxDelta = 15000;
                    uint32_t    bufSize = 4 * 1024;
                    std::string   toString();
                };

                struct SniffParams {
                    Params params;
                };


            public:
                Params      params;
                Gpio        gpio;
                OnSniffed   onSniffed = 0;

            public:
                Sniffer();
                Sniffer(Params params);
                ~Sniffer();

                void init(Params params);
                void uninit();

                
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
                int      getBufSize() {return this->params.bufSize;};
                Delta*   getSniffedDelta() {return this->sniffedDelta;};
                int      getSniffedCount() {return (int)(this->sniffedCount);};
                String   toString();
            public:
                static bool parseParams(neb::CJsonObject* jp, rfir::module::ttl::Sniffer::Params* p);
                static std::string packSniffedCmd(Params* params, const char* data);
            };
        }
    }
}


#endif //__RFIR_MODULE_GPIO_TTL_SNIFFER_H__
