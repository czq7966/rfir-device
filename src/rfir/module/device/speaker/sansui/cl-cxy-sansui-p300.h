#ifndef __RFIR_MODULE_DEVICE_SPEAKER_CL_CXY_SANSUI_P300_H__
#define __RFIR_MODULE_DEVICE_SPEAKER_CL_CXY_SANSUI_P300_H__


#include "sansui.h"


namespace rfir {
    namespace module {
        namespace device {
            namespace speaker {
                class CL_CXY_SANSUI_P300: public SANSUI {
                public:
                    static const uint8_t PIN_SNIFF    = 14;
                    static const uint8_t PIN_SEND     = 4;
                    static const uint8_t PIN_PLAY     = 13;
                public:
                    rfir::module::ttl::Gpio    gpioPlay; 
                    bool doModePinChange();                    
                public:
                    virtual void start(void *) override;
                    virtual void loop() override;                     
                public:
                    virtual rfir::module::ttl::Config::Device* init() override;
                    virtual bool onCmd_get(neb::CJsonObject* pld) override; 
                public:
                    static bool  PlayPinChanged;
                    static void OnPlayPinChange(rfir::module::ttl::Gpio* gpio, int value);                
                };
            }
        }
    }
}



#endif //
