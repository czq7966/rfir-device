#ifndef __RFIR_MODULE_DEVICE_DEVICE_H__
#define __RFIR_MODULE_DEVICE_DEVICE_H__


#include <Arduino.h>
#include "config.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/gpio.h"
#include "rfir/module/ttl/config.h"

namespace rfir {
    namespace module {
        namespace device {
            class Device {
            public:
                typedef std::function<void(void* device, const char* reason)> OnChange;
                std::string name;
                OnChange onChange;
                void emitChange(const char* reason);
            public:
                int timerReport_LastTime = 0;
                int timerReport_Interval = 60 * 1000;            

            public:
                virtual void start(void *);
                virtual void loop();
                virtual void onSetRaw();
                virtual bool loadRaw();      
                virtual bool saveRaw();      
                virtual void doTimerReport(bool reset = false);       
                virtual std::string toBitString();
                virtual std::string toHexString();

            public: // subclass need implement below
                virtual rfir::module::ttl::Config::Device* init();            
                virtual bool setRaw(uint8_t* raw);
                virtual uint8_t* getRaw(int& count);                
                virtual uint16_t* getEncodeRaw(int& count);
                virtual bool onCmd_set(neb::CJsonObject* pld); 
                virtual bool onCmd_get(neb::CJsonObject* pld); 
                virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) ; 
            };
        }
    }
}



#endif //
