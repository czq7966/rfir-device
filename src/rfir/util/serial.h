#ifndef __RFIR_UTIL_Serial_H__
#define __RFIR_UTIL_Serial_H__

#include "platform.h"
#include "map-factory.h"

namespace rfir {
    namespace util {
        class Serial_Configs {
        public:
            MapFactory<int, std::string> configs;
            Serial_Configs();           
          
        };
    }
}
                
extern rfir::util::Serial_Configs GSerial_Configs;

#endif                