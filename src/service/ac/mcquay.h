#ifndef __SERVICE_AC_MCQUAY_H__
#define __SERVICE_AC_MCQUAY_H__


#include <Arduino.h>
#include "module/ac/mcquay.h"

namespace service {
    namespace ac {
        class Mcquay {
        public:
            static void Start(){
                module::ac::Mcquay::Start();
            }
            static bool Loop() {
                return module::ac::Mcquay::Loop();
            }
        };
    }
}



#endif //__SERVICE_AC_MCQUAY_H__
