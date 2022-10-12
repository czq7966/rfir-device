#ifndef __RFIR_UTIL_CONFIG_H__
#define __RFIR_UTIL_CONFIG_H__

#include "platform.h"
#include "reg-table.h"

namespace rfir {
    namespace util {
        class Config {
        public:
            struct IDs
            {
                uint16_t vender = 60000;
            };
        public:
            
        public:
            IDs ids;
            MapFactory<int, int> tables;
          
        };
    }
}
                
extern rfir::util::Config GConfig;

#endif                