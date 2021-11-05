#ifndef __RFIR_MODULE_DEVICE_AC_MCQUAY_H__
#define __RFIR_MODULE_DEVICE_AC_MCQUAY_H__


#include "../../device.h"
#include "rfir/rfir.h"
#include "mcquay-ac.h"



namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class Mcquay: public Device {
                public:
                    static const uint8_t MODE_NONE     = 0b0000;
                    static const uint8_t MODE_DRY      = 0b0001;
                    static const uint8_t MODE_COOL     = 0b0010;
                    static const uint8_t MODE_FAN      = 0b0100;            
                    static const uint8_t MODE_HEAT     = 0b1000;

                public:
                    enum EModePin {
                        FAN = 16,
                        COOL = 12,
                        HEAT = 13,
                        DRY = 5
                    };                    
                public:
                    McquayAC* ac = 0;
                    rfir::module::ttl::Gpio    gpioFan;
                    rfir::module::ttl::Gpio    gpioCool;
                    rfir::module::ttl::Gpio    gpioHeat;
                    rfir::module::ttl::Gpio    gpioDry;
                public:                     
                    Mcquay();
                    ~Mcquay();
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;            
                    virtual bool setRaw(uint8_t* raw) override;
                    virtual uint8_t* getRaw(int& count) override;       
                    virtual uint16_t* getEncodeRaw(int& count);         
                    virtual bool onCmd_set(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                    virtual bool onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) override; 
                public:
                    bool doModePinChange();
                    bool getPower();
                    uint8_t getPinMode();
                    std::string getPinModeStr(int pin);
                    std::string getModeStr(uint8_t mode);
                    std::string getFanStr(uint8_t fan);                    
                public:
                    static int  ModePinChange_GpioPin;
                    static void OnModePinChange(rfir::module::ttl::Gpio* gpio, int value);
                };
            }
        }
    }
}



#endif //
