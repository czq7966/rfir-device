#ifndef __RFIR_UTIL_REG_TABLE_H__
#define __RFIR_UTIL_REG_TABLE_H__

#include "platform.h"
#include "map-factory.h"

namespace rfir {
    namespace util {
        class RegTable {
           
        public:
            MapFactory<int, int> tables;
            MapFactory<int, int> sizes;
            int                  strMinNum = 60000;
            int                  strMaxNum = 60499;
            int                  bytesMinNum = 60500;
            int                  bytesMaxNum = 60999;            
        public:
            bool encode(char* buf, int& size, std::list<int>& ids);
            bool encode(char* buf, int& size);
            bool decode(const char* buf, int size, std::list<int>& ids);   
            bool decode(const char* buf, int size);     
            bool merge(RegTable* regTable, std::list<int>& ids);
            bool load(const char* buf, int size, std::list<int>& ids);
            bool load(const char* buf, int size, rfir::util::MapFactory<int, int>& ids);
            void clear();
            void dump();
        };
    }
}

#endif                