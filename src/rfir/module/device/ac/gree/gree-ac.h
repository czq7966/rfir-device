#ifndef __MODULE_AC_GREE_AC_H__
#define __MODULE_AC_GREE_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"
#include "ir_Gree.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class GreeAC {  
                public:
                    static const uint16_t KGreeHdrMark = 9000;
                    static const uint16_t KGreeHdrSpace = 4500;  ///< See #684 & real example in unit tests
                    static const uint16_t KGreeBitMark = 620;
                    static const uint16_t KGreeOneSpace = 1600;
                    static const uint16_t KGreeZeroSpace = 540;
                    static const uint16_t KGreeMsgSpace = 19980;  ///< See #1508, #386, & Kelvinator
                    static const uint8_t  KGreeBlockFooter = 0b010;
                    static const uint8_t  KGreeBlockFooterBits = 3;      
                    static const uint16_t KGreeEncodeRawLength =    2 + kGreeStateLength / 2 * 8 * 2 + KGreeBlockFooterBits * 2 + 2 + 
                                                                    kGreeStateLength / 2 * 8 * 2 + 2 +
                                                                    2 + kGreeStateLength / 2 * 8 * 2 + KGreeBlockFooterBits * 2 + 2;


                public:
                    GreeAC();
                    IRGreeAC* ac;
                    
                    uint16_t encodeRaw[KGreeEncodeRawLength];


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
