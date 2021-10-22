#ifndef __MODULE_AC_MCQUAY_H__
#define __MODULE_AC_MCQUAY_H__


#include <Arduino.h>
#include "mcquay.h"
#include "mcquay_ac.h"
#include "rfir/util/cjson/CJsonObject.hpp"

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

        public:
            enum EModePin {
                FAN = 16,
                COOL = 12,
                HEAT = 13,
                DRY = 5
            };

        public:
            static bool getPower();
            static uint8_t getPinMode();
            static std::string toString();
        };
    }
}



#endif //__MODULE_AC_MCQUAY_H__
