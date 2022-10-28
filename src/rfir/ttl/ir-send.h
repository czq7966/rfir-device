#ifndef RFIR_TTL_IR_SEND_H
#define RFIR_TTL_IR_SEND_H

#include <stdint.h>


namespace rfir {
    namespace ttl {
        class IRSend {
        public:
            #if defined(ESP32)
            static const int8_t PeriodOffset = -2;
            #elif (defined(ESP8266) && F_CPU == 160000000L) 
            static const int8_t PeriodOffset = -2;
            #else
            static const int8_t PeriodOffset = -5;
            #endif
            static const uint8_t DutyDefault = 50; 
            const uint8_t DutyMax = 100;  
            const uint16_t MaxAccurateUsecDelay = 16383;
            const uint32_t DefaultMessageGap = 100000;


        public:
            explicit IRSend(uint16_t IRsendPin, bool inverted = false,
                            bool use_modulation = true);
            void begin();
            void enableIROut(uint32_t freq, uint8_t duty = DutyDefault);
            void _delayMicroseconds(uint32_t usec);
            uint16_t mark(uint16_t usec);
            void space(uint32_t usec);
            int8_t calibrate(uint16_t hz = 38000U);
            void sendRaw(const uint16_t buf[], const uint16_t len, const uint16_t hz);

        protected:
            uint8_t outputOn;
            uint8_t outputOff;
            void ledOff();
            void ledOn();
            private:
            uint16_t onTimePeriod = 0;
            uint16_t offTimePeriod = 0;
            uint16_t pin = 0;
            int8_t periodOffset = 0;
            uint8_t _dutycycle = 0;
            bool modulation = 0;
            uint32_t calcUSecPeriod(uint32_t hz, bool use_offset = true);
        };
    }

}
#endif 
