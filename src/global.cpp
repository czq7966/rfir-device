#include "global.h"
#include "config.h"



bool Global::_IsPowerHardReset = false;  

void  Global::CheckPowerHardReset() {
        _IsPowerHardReset = false;
#ifdef PWR_RESET_CHK_PIN
        int pin = PWR_RESET_CHK_PIN;
        pinMode(pin, INPUT);
        _IsPowerHardReset = (digitalRead(pin) == 0);
        pinMode(pin, OUTPUT);
        digitalWrite(pin, 1);
#endif  

    }   

void Global::Init() {
        CheckPowerHardReset();
}

bool Global::IsPowerHardReset() {
        return _IsPowerHardReset;
}

