#ifndef __MODULE_AC_OKONOFF_AC_H__
#define __MODULE_AC_OKONOFF_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"

#define GETBITS8(data, offset, size) \
    (((data) & (((uint8_t)UINT8_MAX >> (8 - (size))) << (offset))) >> (offset))

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class OkonoffAC {        
            public:
                static const uint8_t KOkonoffStateLength = 14;            
                //Header
                static const uint32_t KOkonoffHeader  = 0x7887a965;
                //Sep
                static const uint8_t KOkonoffSep      = 0x00;

                //Footer
                static const uint32_t KOkonoffFooter  = 0x00ff00ff;

                //Power
                static const uint8_t KOkonoffPowerOn  = 0b1000;
                static const uint8_t KOkonoffPowerOff = 0b0000;

                //Mode
                static const uint8_t KOkonoffModeHeat = 0b0001;
                static const uint8_t KOkonoffModeCool = 0b0011;

                //Fan
                static const uint8_t KOkonoffFanAuto = 0b000;
                static const uint8_t KOkonoffFanLow  = 0b001;
                static const uint8_t KOkonoffFanMed  = 0b010;
                static const uint8_t KOkonoffFanHigh  = 0b011;

                //Temp
                static const uint8_t KOkonoffMinTempC = 5;   // Celsius
                static const uint8_t KOkonoffMaxTempC = 35;  // Celsius


            public:
                static const uint8_t    KOkonoffEncodeRawLength = KOkonoffStateLength * 8 * 2 + 4;
                static const uint16_t   KOkonoffEncodeHeaderMark = 3500;
                static const uint16_t   KOkonoffEncodeHeaderSpace = 1700;
                static const uint16_t   KOkonoffEncodeOneMark = 560;
                static const uint16_t   KOkonoffEncodeOneSpace = 1150;
                static const uint16_t   KOkonoffEncodeZeroMark = 560;
                static const uint16_t   KOkonoffEncodeZeroSpace = 340;
                static const uint16_t   KOkonoffEncodeFooterMark = 560;
                static const uint16_t   KOkonoffEncodeFooterSpace = 10000;

            public:
                union Protocol{
                    uint8_t remote_state[KOkonoffStateLength];  
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
                OkonoffAC();
                Protocol protocol;
                uint16_t encodeRaw[KOkonoffEncodeRawLength];
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

                void checksum(const uint16_t length = KOkonoffStateLength);
                bool validsum();
                void fixup();

                std::string toString();
                std::string toBitString();
                std::string toHexString();

                uint16_t*   getEncodeRaw();      
                std::string getEncodeString();

                static uint8_t calcBlockChecksum(const uint8_t* block, const uint16_t length = KOkonoffStateLength);
                static bool validChecksum(const uint8_t state[],const uint16_t length = KOkonoffStateLength);
                static uint8_t dec2hex(const uint8_t dec);
                static uint8_t hex2dec(const uint8_t hex);                        
        
            };
            }
        }
    }

}

#endif //__MODULE_AC_Okonoff_H__
