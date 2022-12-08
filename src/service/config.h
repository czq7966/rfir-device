#ifndef SERVICE_CONFIG_H
#define SERVICE_CONFIG_H

#include "device.h"


//SYSTEM
#ifndef CHIP_ID_PREFIX
    #ifdef ESP8266
    #define CHIP_ID_PREFIX              "ESP8266x" 
    #else
    #define CHIP_ID_PREFIX              "ESP32x" 
    #endif
#endif

#include "device.h"



#include "cmds/cmd/reg-table.h"
#include "rfir/util/event-emitter.h"
#include "rfir/util/button.h"
#include "rfir/util/map-factory.h"

namespace service {
    class Config {     
    public:
        struct Events {
            rfir::util::Event loaded;
            rfir::util::Event saved;
            rfir::util::Event ready;
        };

        struct Params {
            const char* filename = 0;
            char*       buf = 0;
            size_t      bufsize = 0;
        };      
    public: 
        Events events;
        Params params;       
        rfir::util::MapFactory<int, int> saved;
        rfir::util::Button::KeyTime keyTimeResetConfig;
    public:
        Config();
        ~Config();
    
    public:
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
