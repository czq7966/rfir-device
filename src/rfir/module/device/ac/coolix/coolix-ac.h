#ifndef __MODULE_AC_COOLIX_AC_H__
#define __MODULE_AC_COOLIX_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class CoolixAC {        
            public:
                static const uint8_t KCoolixStateLength = 14;            
                //Header
                static const uint32_t KCoolixHeader  = 0x7887a965;
                //Sep
                static const uint8_t KCoolixSep      = 0x00;

                //Footer
                static const uint32_t KCoolixFooter  = 0x00ff00ff;

                //Power
                static const uint8_t KCoolixPowerOn  = 0b1000;
                static const uint8_t KCoolixPowerOff = 0b0000;

                //Mode
                static const uint8_t KCoolixModeHeat = 0b0001;
                static const uint8_t KCoolixModeCool = 0b0011;

                //Fan
                static const uint8_t KCoolixFanAuto = 0b000;
                static const uint8_t KCoolixFanLow  = 0b001;
                static const uint8_t KCoolixFanMed  = 0b010;
                static const uint8_t KCoolixFanHigh  = 0b011;

                //Temp
                static const uint8_t KCoolixMinTempC = 5;   // Celsius
                static const uint8_t KCoolixMaxTempC = 35;  // Celsius


            public:
                static const uint8_t    KCoolixEncodeRawLength = KCoolixStateLength * 8 * 2 + 4;
                static const uint16_t   KCoolixEncodeHeaderMark = 3500;
                static const uint16_t   KCoolixEncodeHeaderSpace = 1700;
                static const uint16_t   KCoolixEncodeOneMark = 560;
                static const uint16_t   KCoolixEncodeOneSpace = 1150;
                static const uint16_t   KCoolixEncodeZeroMark = 560;
                static const uint16_t   KCoolixEncodeZeroSpace = 340;
                static const uint16_t   KCoolixEncodeFooterMark = 560;
                static const uint16_t   KCoolixEncodeFooterSpace = 10000;

            public:
                union Protocol{
                    uint8_t remote_state[KCoolixStateLength];  
                    struct {
                        // Byte 0-3                       // 头部，固定值：0x 65 9a 87 78
                        uint8_t H1;      
                        uint8_t H2;
                        uint8_t H3;
                        uint8_t H4;
                        // Byte 4
                        uint8_t S1              :8;       // 每4个字节分隔符 0x00
                        // Byte 5
                        uint8_t Power           :4;       // 低4位=开关：0b0000=关; 0b1000=开
                        uint8_t Mode            :4;       // 高4位=模式：0b0001=制热；0b0011=制冷                        
                        // Byte 6
                        uint8_t N1              :8;       // 第6字符取反
                        // Byte 7
                        uint8_t Fan             :3;       // 低3位=风速：0b000=自动；低速=0b001; 中速=0b010; 高速=0b011
                        uint8_t Temp1           :1;       // 第4位=温度 * 15 
                        uint8_t Temp2           :4;       // 高4位=温度 + 5
                        // Byte 8                        
                        uint8_t N2              :8;       // 第8字符取反
                        // Byte 9
                        uint8_t S2              :8;       // 每4个字节分隔符 0x00
                        // Byte 10-13                     // 固定值：0x FF 00 FF 00
                        uint8_t F1;
                        uint8_t F2;
                        uint8_t F3;
                        uint8_t F4;
                    };
                };            

            public:
                CoolixAC();
                Protocol protocol;
                uint16_t encodeRaw[KCoolixEncodeRawLength];
                std::function<void(void*)> onSetRaw;
                void    setHeader();
                void    setFooter();
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

                static uint8_t calcBlockChecksum(const uint8_t* block, const uint16_t length = KCoolixStateLength);
                static bool validChecksum(const uint8_t state[],const uint16_t length = KCoolixStateLength);
                static uint8_t dec2hex(const uint8_t dec);
                static uint8_t hex2dec(const uint8_t hex);                        
        
            };
            }
        }
    }

}

#endif //__MODULE_AC_Coolix_H__
