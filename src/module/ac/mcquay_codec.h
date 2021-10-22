#ifndef __MODULE_AC_MCQUAY_AC_H__
#define __MODULE_AC_MCQUAY_AC_H__


#include <Arduino.h>
#include <string>

namespace module {
    namespace ac {
        class McquayCodec {
        public:
            static const uint8_t KMcQuayStateLength = 8;
            //Header
            static const uint8_t KMcQuayHeader  = 0x16;

            //Mode
            static const uint8_t KMcQuayAuto = 0b0000;
            static const uint8_t KMcQuayDry  = 0b0001;
            static const uint8_t KMcQuayCool = 0b0010;
            static const uint8_t KMcQuayFan  = 0b0100;
            static const uint8_t KMcQuayHeat = 0b1000;
            
            //Fan
            static const uint8_t KMcQuayFanAuto = 0b0001;
            static const uint8_t KMcQuayFanMin  = 0b1000;
            static const uint8_t KMcQuayFanMed  = 0b0100;
            static const uint8_t KMcQuayFanMax  = 0b0010;

            //Swing
            static const uint8_t KMcQuaySwingOpen  = 0b1;
            static const uint8_t KMcQuaySwingClose  = 0b0;

            //Sleep
            static const uint8_t KMcQuaySleepOpen  = 0b1;
            static const uint8_t KMcQuaySleepClose  = 0b0;

            //Temp
            static const uint8_t KMcQuayMinTempC = 16;  // Celsius
            static const uint8_t KMcQuayMaxTempC = 30;  // Celsius

            static const uint16_t KMcQuayTimerMax = 24 * 60;        
        public:
        
                    
     
        };
    }
}



#endif //__MODULE_AC_MCQUAY_H__
