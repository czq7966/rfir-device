#ifndef __MODULE_AC_MCQUAY_AC_H__
#define __MODULE_AC_MCQUAY_AC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"

#define GETBITS8(data, offset, size) \
    (((data) & (((uint8_t)UINT8_MAX >> (8 - (size))) << (offset))) >> (offset))

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class McquayAC {        
            public:
                static const uint8_t KMcQuayStateLength = 8;            
                //Header
                static const uint8_t KMcQuayHeader  = 0x16;            

                //Mode
                static const uint8_t KMcQuayModeNone = 0b0000;
                static const uint8_t KMcQuayModeDry  = 0b0001;
                static const uint8_t KMcQuayModeCool = 0b0010;
                static const uint8_t KMcQuayModeFan  = 0b0100;
                static const uint8_t KMcQuayModeHeat = 0b1000;
                
                //Fan
                static const uint8_t KMcQuayFanAuto = 0b0001;
                static const uint8_t KMcQuayFanMin  = 0b1000;
                static const uint8_t KMcQuayFanMed  = 0b0100;
                static const uint8_t KMcQuayFanMax  = 0b0010;

                //Swing
                static const uint8_t KMcQuaySwingOpen  = 0b1;
                static const uint8_t KMcQuaySwingClose  = 0b0;

                //Sleep
                static const uint8_t KMcQuaySleepOpen  = 0b1;
                static const uint8_t KMcQuaySleepClose  = 0b0;

                //Temp
                static const uint8_t KMcQuayMinTempC = 16;  // Celsius
                static const uint8_t KMcQuayMaxTempC = 30;  // Celsius

                static const uint16_t KMcQuayTimerMax = 24 * 60;  
            public:
                static const uint8_t    KMcQuayEncodeRawLength = 134;
                static const uint16_t   KMcQuayEncodeHeaderMark = 4690;
                static const uint16_t   KMcQuayEncodeHeaderSpace = 2610;
                static const uint16_t   KMcQuayEncodeOneMark = 375;
                static const uint16_t   KMcQuayEncodeOneSpace = 910;
                static const uint16_t   KMcQuayEncodeZeroMark = 375;
                static const uint16_t   KMcQuayEncodeZeroSpace = 390;
                static const uint16_t   KMcQuayEncodeFooterMark = 375;
                static const uint16_t   KMcQuayEncodeFooterSpace = 20470;

            public:
                union Protocol{
                    uint8_t remote_state[KMcQuayStateLength];  
                    struct {
                        // Byte 0
                        uint8_t Header          :8;       // 固定值：0b00010110（0x16）
                        // Byte 1    
                        uint8_t Mode            :4;       // 低4位=模式：0b0000=除湿；0b0001=吹风；0b1001=制冷；其它值待续
                        uint8_t Fan             :4;       // 高4位=风速：0b1000=低风；0b0100=中风；0b0010=高风；0b0001=自动
                        // Byte 2    
                        uint8_t Minute          :8;       // 高4位=分钟之十位的值，如36分钟的3，=0b0011// 低4位=分钟之个位的值，如36分钟的6，=0b0110
                        // Byte 3
                        uint8_t Hour            :8;       // 高4位=小时之十位的值，如14时的1，=0b0001  // 低4位=小时之个位的值，如14时的4，=0b0100
                        // Byte 4
                        uint8_t OpenTime        :7;       //定时开时间，具体说明待续
                        uint8_t TimingOpen      :1;       //第8位="定时开"功能开关，1=开启"定时开"功能；0=关闭"定时开"功能
                        // Byte 5
                        uint8_t CloseTime       :7;       //定时关时间，具体说明待续
                        uint8_t TimingClose     :1;       //第8位="定时关"功能开关，1=开启"定时关"功能；0=关闭"定时开"功能    
                        // Byte 6
                        uint8_t Temp            :8;       //高4位=温度之十位的值，如25度的2，=0b0010   //低4位=温度之个位的值，如25时的5，=0b0101
                        // Byte 7
                        uint8_t Swing           :1;       //摇摆开关，1=开；0=关
                        uint8_t Sleep           :1;       //睡眠开关，1=开；0=关
                        uint8_t Unknown         :1;       //未知，保留
                        uint8_t PowerSwitch     :1;       //开、关转换
                        uint8_t Sum             :4;       //高4位=校验值=前7个字节的高4位 + 低4位 + 第8字节低4位的和，取总和的低4位
                    };
                };            

            public:
                Protocol protocol;
                uint16_t encodeRaw[KMcQuayEncodeRawLength];
                std::function<void(void*)> onSetRaw;
                void    setHeader(const uint8_t header);
                uint8_t getHeader();
                void    setTemp(const uint8_t temp, const bool fahrenheit = false);
                uint8_t getTemp();
                void    setFan(const uint8_t speed);
                uint8_t getFan();
                void    setMode(const uint8_t new_mode);
                uint8_t getMode();
                void    setSleep(const bool on);
                bool    getSleep();
                void    setSwing(const bool on);
                bool    getSwing();  
                void    setPowerSwitch(const bool on);
                bool    getPowerSwitch();              
                uint8_t getHour();
                void    setHour(const uint8_t hour);
                uint8_t getMinute();
                void    setMinute(const uint8_t minute);  
                uint8_t getPinMode();

                void setTimerEnabled(const bool on);
                bool getTimerEnabled();   

                uint8_t* getRaw(void);
                void    setRaw(const uint8_t new_code[]);

                void checksum(const uint16_t length = KMcQuayStateLength);
                bool validsum();
                void fixup();

                std::string toString();
                std::string toBitString();
                std::string toHexString();

                uint16_t*   getEncodeRaw();      
                std::string getEncodeString();

                static uint8_t calcBlockChecksum(const uint8_t* block, const uint16_t length = KMcQuayStateLength);
                static bool validChecksum(const uint8_t state[],const uint16_t length = KMcQuayStateLength);
                static uint8_t dec2hex(const uint8_t dec);
                static uint8_t hex2dec(const uint8_t hex);                        
        
            };
            }
        }
    }

}

#endif //__MODULE_AC_MCQUAY_H__