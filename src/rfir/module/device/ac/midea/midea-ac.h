#ifndef __RFIR_MODULE_DEVICE_AC_MIDEAAC_H__
#define __RFIR_MODULE_DEVICE_AC_MIDEAAC_H__


#include <Arduino.h>
#include <string>
#include "rfir/util/platform.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace ac {
                class MideaAC {  
                public:
                    static const uint16_t KBits = 48;
                    // static const uint16_t KHdrMark = 4350;
                    // static const uint16_t KHdrSpace = 4450;
                    // static const uint16_t KOneMark = 440;
                    // static const uint16_t KOneSpace = 1740;
                    // static const uint16_t KZeroMark = 420;
                    // static const uint16_t KZeroSpace = 650;
                    // static const uint16_t KFtrMark = 440;
                    // static const uint16_t KFtrSpace = 5300;

                    static const uint16_t KHdrMark = 4390;
                    static const uint16_t KHdrSpace = 4420;
                    static const uint16_t KOneMark = 570;
                    static const uint16_t KOneSpace = 1620;
                    static const uint16_t KZeroMark = 570;
                    static const uint16_t KZeroSpace = 520;
                    static const uint16_t KFtrMark = 570;
                    static const uint16_t KFtrSpace = 5300;

                    static const uint16_t KEncodeRawLength =  KBits * 2 + 4;
                    static const uint8_t  KBytesLength = KBits / 8;     

                    union Protocol{
                        uint64_t remote_state;  
                        struct {
                            // Byte 0
                            uint8_t BT0;             // =0b10110010
                            // Byte 1
                            uint8_t BT1;             // ~BT0
                            // Byte 2 
                            uint8_t BT2_1   :2;     // =0b11
                            uint8_t Power   :1;     // on=0b1; off=0b0, temp=0b1110,
                            uint8_t BT2_2   :2;     // =0b11
                            uint8_t Fan     :3;     // auto=0b101, low=0b100, med=0b010,high=0b001                            
                            
                            // Byte 3
                            uint8_t BT3;             //~B2
                            // Byte 4
                            uint8_t BT4_1   :2;     // =0b00;                            
                            uint8_t Mode    :2;     // auto=0b10; cool=0b00; dry=0b01; heat=0b11; fan=0b01+Temp=none;
                            uint8_t Temp    :4;     // none=0b1110;
                            
                            // Byte 5
                            uint8_t BT5;             //~B4        
                            // Byte 6
                            uint8_t BT6;             
                            // Byte 7
                            uint8_t BT7;             
                        };
                    };   

                    const uint8_t P_BT0         = 0b10110010;                    
                    // const uint8_t P_BT1         = 0b01001101;
                    const uint8_t P_BT2_1       = 0b11;
                    const uint8_t P_BT2_2       = 0b11;
                    const uint8_t P_BT4_1       = 0b00;

                    const uint8_t P_Fan_Auto    = 0b101;
                    const uint8_t P_Fan_Low     = 0b100;
                    const uint8_t P_Fan_Med     = 0b010;
                    const uint8_t P_Fan_High    = 0b001;

                    const uint8_t P_Power_On    = 0b1;
                    const uint8_t P_Power_Off   = 0b0;
                    const uint64_t P_Close      = 0x1fe0847b4db2; 

                    const uint8_t P_Temp[14]    = {0b0000, 0b0001, 0b0011, 0b0010, 0b0110, 0b0111, 0b0101, 0b0100, 0b1100, 0b1101, 0b1001, 0b1000, 0b1010, 0b1011};
                    const uint8_t P_Temp_Min    = 17;
                    const uint8_t P_Temp_Max    = 30;
                    const uint8_t P_Temp_None   = 0b1110;

                    const uint8_t P_Mode_Auto   = 0b10;
                    const uint8_t P_Mode_Cool   = 0b00;
                    const uint8_t P_Mode_Dry    = 0b01;
                    const uint8_t P_Mode_Heat   = 0b11;
                    const uint8_t P_Mode_Fan    = 0b01;

                public:
                    Protocol protocol = {};  
                    Protocol protocol_close = {};
                    uint16_t encodeRaw[KEncodeRawLength];
                    MideaAC();
                public:


                    std::string toString();
                    std::string toBitString();
                    std::string toHexString();

                    uint16_t*   getEncodeRaw(int& count);      
                    // std::string getEncodeString();

                    uint8_t     getPower();
                    void        setPower(uint8_t v);
                    uint8_t     getMode();
                    void        setMode(uint8_t v);
                    uint8_t     getTemp();
                    void        setTemp(uint8_t v);
                    uint8_t     getFan();
                    void        setFan(uint8_t v);                

                    void        fixUp();

                    bool setRaw(uint8_t* raw);
                    uint8_t* getRaw(int& count);                     
            
                };
            }
        }
    }

}

#endif //__MODULE_AC_Coolix_H__
