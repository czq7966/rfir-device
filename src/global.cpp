#include "global.h"
#include "config.h"



bool Global::_IsPowerHardReset = false;  

void  Global::CheckPowerHardReset() {
        _IsPowerHardReset = false;
#ifdef PWR_RESET_CHK_PIN
        int pin = PWR_RESET_CHK_PIN;
        pinMode(pin, INPUT);
        int v1 = digitalRead(pin);
        _IsPowerHardReset = (v1 == PWR_RESET_CHK_VALUE);
        if (_IsPowerHardReset) {
            pinMode(pin, OUTPUT);
            digitalWrite(pin, !PWR_RESET_CHK_VALUE);
        }

        printf("IsPowerHardReset=%d , %d, %d \r\n", pin, v1, _IsPowerHardReset);

#endif  

    }   

void Global::Init() {
        CheckPowerHardReset();
}

bool Global::IsPowerHardReset() {
        return _IsPowerHardReset;
}

JLed    GJLed = JLed(LED_PIN);