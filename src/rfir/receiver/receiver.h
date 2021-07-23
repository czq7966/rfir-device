//
// Created by Administrator on 2020/12/10.
//

#ifndef __RFIR_RECEIVER_RECEIVER_H__
#define __RFIR_RECEIVER_RECEIVER_H__

#include "rfir/util/platform.h"

namespace rfir {
    namespace receiver {
        struct Args {
            uint8_t pin;
            uint32_t bufSize = 1024 * 9  ;
            uint32_t minTTLCount = 4 * 8;
            uint32_t minTTLTime = 20 * 5;            
        };
        
        class Receiver {
        private:
            Args args;


        public:
            Receiver(Args args);
            ~Receiver();

        };
    }
}


#endif //__RFIR_RECEIVER_FILTER_H__
