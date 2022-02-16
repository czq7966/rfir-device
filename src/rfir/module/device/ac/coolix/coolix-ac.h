#ifndef __MODULE_AC_COOLIX_AC_H__
#define __MODULE_AC_COOLIX_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"
#include "ir_Coolix.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CoolixAC {        
                public:
                    static const uint8_t KCoolixStateLength = 6;            
                public:
                    static const uint16_t   KCoolixTick = 276;
                    static const uint8_t    KCoolixEncodeRawLength = KCoolixStateLength * 8 * 2 + 4;
                    static const uint16_t   KCoolixEncodeHeaderMark = 17 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeHeaderSpace = 16 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeOneMark = 2 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeOneSpace = 6 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeZeroMark = 2 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeZeroSpace = 2 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeFooterMark = 2 * KCoolixTick;
                    static const uint16_t   KCoolixEncodeFooterSpace = 19 * KCoolixTick;

                public:
                    union Protocol{
                        uint8_t remote_state[KCoolixStateLength];  
                    };            

                public:
                    CoolixAC();
                    IRCoolixAC* ac;
                    
                    Protocol protocol;
                    uint16_t encodeRaw[KCoolixEncodeRawLength];
                    std::function<void(void*)> onSetRaw;
                    void    setTemp(const uint8_t temp, const bool fahrenheit = false);
                    uint8_t getTemp();
                    void    setFan(const uint8_t speed);
                    uint8_t getFan();
                    void    setMode(const uint8_t new_mode);
                    uint8_t getMode();
                    void    setPower(const bool on);
                    bool    getPower();              

                    uint8_t* getRaw(void);
                    void    setRaw(const uint8_t new_code[]);

                    void checksum(const uint16_t length = KCoolixStateLength);
                    bool validsum();
                    void fixup();

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
