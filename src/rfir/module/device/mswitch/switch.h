#ifndef __RFIR_MODULE_DEVICE_MSWITCH_Switch_H__
#define __RFIR_MODULE_DEVICE_MSWITCH_Switch_H__


#include "../device.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace mswitch {
                class Switch: public Device {
                public:
                    static const std::string SWITCH_ON;
                    static const std::string SWITCH_OFF;
                    static const std::string SWITCH_TOGGLE;                    
                public:
                    struct Params {
                        String      name;
                        String      rfirToggle;
                        bool        enabled;
                        int8_t      pinIn;
                        int8_t      pinOut;
                        bool        default_v;
                    };

                    Params params;
                    rfir::module::ttl::Gpio    gpioIn;
                    rfir::module::ttl::Gpio    gpioOut;                      
                public:
                    Switch();
                    bool getState();
                    std::string getStateStr();
                    void setState(bool state);
                    // bool doPinChange();             
                    // void setState(std::string key,  std::string value);
                    // std::string getState(std::string key);
                    // rfir::module::ttl::Gpio* getGpio(std::string key);
                    // void setMaxSwitchCount(uint16_t count);
                public:
                    virtual void start(void *) override;
                    virtual void loop() override; 

                public:
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                public:
                    static void* ChangedSwitch;
                    static void  OnPinChange(rfir::module::ttl::Gpio* gpio, int value);
                };
            }
        }
    }
}



#endif //
