#ifndef __RFIR_MODULE_DEVICE_AC_MIDEA_AC_H__
#define __RFIR_MODULE_DEVICE_AC_MIDEA_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"
#include "ir_Midea.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class MideaAC {  
                public:
                    static const uint16_t KMideaHdrMark = 9000;
                    static const uint16_t KMideaHdrSpace = 4500;  ///< See #684 & real example in unit tests
                    static const uint16_t KMideaBitMark = 620;
                    static const uint16_t KMideaOneSpace = 1600;
                    static const uint16_t KMideaZeroSpace = 540;
                    static const uint16_t KMideaMsgSpace = 19980;  ///< See #1508, #386, & Kelvinator
                    static const uint8_t  KMideaBlockFooter = 0b010;
                    static const uint8_t  KMideaBlockFooterBits = 3;      
                    static const uint16_t KMideaEncodeRawLength =  0;
                    static const uint8_t  KMideaStateLength = kMideaBits / 8;      
                    


                public:
                    MideaAC();
                    IRMideaAC* ac;
                    
                    uint16_t encodeRaw[KMideaEncodeRawLength];


                    std::string toString();
                    std::string toBitString();
                    std::string toHexString();

                    uint16_t*   getEncodeRaw();      
                    std::string getEncodeString();
            
                };
            }
        }
    }

}

#endif //__MODULE_AC_Coolix_H__
