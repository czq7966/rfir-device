//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_RECEIVER_TTL_FILTER_H__
#define __RFIR_RECEIVER_TTL_FILTER_H__

#include "rfir/util/platform.h"

namespace rfir {
    namespace receiver {
        struct TTLData
        {
            bool ttl; 
            timeval time;
        };
        
        class TTLFilter {
        private:
            int bufSize = 1024 * 9  ;
            int minTTLCount = 50;
            int minTTLTime = 100;

            int currTTLCount = 0;

            TTLData* dataBuf = 0;
            TTLData  preElapseTTL = {};
        public:
            uint32_t stashCount = 0;
            uint32_t pushCount = 0;
            uint32_t popCount = 0;

            timeval time;


        public:
            TTLFilter();
            ~TTLFilter();

            
            bool pushTTL(bool ttl);
            bool popTTL(TTLData* ttl);

            bool getTTLElapse(TTLData ttl, TTLData* data);

        };
    }
}


#endif //__RFIR_RECEIVER_TTL_FILTER_H__
