#ifndef __SERVICE_AC_MCQUAY_H__
#define __SERVICE_AC_MCQUAY_H__


#include <Arduino.h>

namespace service {
    namespace ac {
        class Mcquay {
        public:
            static void Start(){}
            static bool Loop() {
                return false;
            }
        };
    }
}



#endif //__MODULE_AC_MCQUAY_H__
