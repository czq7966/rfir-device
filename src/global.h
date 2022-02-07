#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "Arduino.h"
#include "config.h"

class Global {
private:
    static bool _IsPowerHardReset;  
protected:
    static void  CheckPowerHardReset();
public:
    static void Init();
    static bool IsPowerHardReset();
};


#endif