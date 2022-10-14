#ifndef SERVICE_CONFIG_H
#define SERVICE_CONFIG_H

#include "config-device.h"


//SYSTEM
#ifndef CHIP_ID_PREFIX
    #ifdef ESP8266
    #define CHIP_ID_PREFIX              "ESP8266x" 
    #else
    #define CHIP_ID_PREFIX              "ESP32x" 
    #endif
#endif

//设备基类配置
#ifdef DEVICE_BASE
#include "config/base.h"
#endif



#include "cmds/cmd/reg-table.h"
#include "rfir/util/event-emitter.h"
#include "rfir/util/button.h"

namespace service {
    class Config {     
    public:
        struct Events {
            rfir::util::Event loaded;
            rfir::util::Event saved;
        };

        struct Params {
            const char* filename = 0;
            char*       buf = 0;
            size_t      bufsize = 0;
        };      
    public: 
        Events events;
        Params params;       
        rfir::util::Button::KeyTime keyTimeResetConfig = {pressed: 10000, released: 15000};
    
    public:
        Config();
        
        void init();
        void fixUp();
        void load();
        void save();
        void save(std::list<int> ids);
        void resetConfig(bool restart = true);
    public:
        int getSerialBaud();
        void setSerialBaud(int value);
        int getSerialConfig();
    };

}
                
extern service::Config GConfig;

#endif                