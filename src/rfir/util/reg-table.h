#ifndef __RFIR_UTIL_REG_TABLE_H__
#define __RFIR_UTIL_REG_TABLE_H__

#include "platform.h"
#include "map-factory.h"

namespace rfir {
    namespace util {
        class RegTable {
        public:
            struct IDs
            {
                uint16_t vender = 60000;
            };
            
        public:
            IDs ids;
            MapFactory<int, int> tables;          
        };
    }
}
                
extern rfir::util::RegTable GRegTable;

#endif                