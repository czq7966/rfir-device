#ifndef __RFIR_UTIL_Serial_H__
#define __RFIR_UTIL_Serial_H__

#include "platform.h"
#include "map-factory.h"

namespace rfir {
    namespace util {
        class SerialConfigs {
        public:
            int baud;
            int data;
            int stop;
            int sum;
        public:
            MapFactory<int, const char*> configs;
            SerialConfigs();    
        public:
            void add(int key, const char* value);

            int getSerialBaud();
            int getSerialConfig();
            int getSerialData();
            int getSerialStop();
            int getSerialSum();

            void setSerialBaud(int);
            void setSerialConfig(int);
            void setSerialConfig(const char*);
            void setSerialData(int);
            void setSerialStop(int);
            void setSerialSum(int);

        };
    }
}
                
extern rfir::util::SerialConfigs GSerialConfigs;

#endif                