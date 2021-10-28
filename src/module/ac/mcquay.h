#ifndef __MODULE_AC_MCQUAY_H__
#define __MODULE_AC_MCQUAY_H__


#include <Arduino.h>
#include "mcquay.h"
#include "mcquay_ac.h"
#include "rfir/util/cjson/CJsonObject.hpp"
#include "rfir/module/ttl/gpio.h"
#include "service/cmds/cmd.h"

namespace module {
    namespace ac {
        class Mcquay {
        public:
            static const u_int8_t MODE_NONE     = 0b0000;
            static const u_int8_t MODE_DRY      = 0b0001;
            static const u_int8_t MODE_COOL     = 0b0010;
            static const u_int8_t MODE_FAN      = 0b0100;            
            static const u_int8_t MODE_HEAT     = 0b1000;
            static McquayAC* AC;
            static rfir::module::ttl::Gpio*    GpioFan;
            static rfir::module::ttl::Gpio*    GpioCool;
            static rfir::module::ttl::Gpio*    GpioHeat;
            static rfir::module::ttl::Gpio*    GpioDry;

        public:
            enum EModePin {
                FAN = 16,
                COOL = 12,
                HEAT = 13,
                DRY = 5
            };

        public:
            static void Start();
            static bool Loop();
        public:
            static rfir::module::ttl::Gpio::OnChange OnModeChange;
            static void OnModePinChange(rfir::module::ttl::Gpio* gpio, int value);       
            static void OnModePinChange_Cool();
            static void OnModePinChange_Dry();
            static void OnModePinChange_Fan();
            static void OnModePinChange_Heat();
            static void DoModePinChange();
            static void DoTimerReport(bool reset = false);
            static void OnSetRaw(void*);
            static bool InitRaw();
        public:
            static bool GetPower();
            static uint8_t GetPinMode();
            static std::string GetPinModeStr(int pin);
            static std::string GetModeStr(uint8_t mode);
            static std::string GetFanStr(uint8_t fan);
            static std::string ToString();   
                 
        };
    }
}



#endif //__MODULE_AC_MCQUAY_H__
