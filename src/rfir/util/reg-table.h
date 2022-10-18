#ifndef __RFIR_UTIL_REG_TABLE_H__
#define __RFIR_UTIL_REG_TABLE_H__

#include "platform.h"
#include "map-factory.h"

namespace rfir {
    namespace util {
        class RegTable {
           
        public:
            MapFactory<int, int> tables;   
            int                  charMinNum = 60000;
            int                  charMaxNum = 60999;
        public:
            bool encode(char* buf, int& size, std::list<int>& ids);
            bool encode(char* buf, int& size);
            bool decode(const char* buf, int size, std::list<int>& ids, bool createKey = false);   
            bool decode(const char* buf, int size, bool createKey = false);     
            bool merge(RegTable* regTable, std::list<int>& ids,  bool createKey = false);
        };
    }
}

#endif                