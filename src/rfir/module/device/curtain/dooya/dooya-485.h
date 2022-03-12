#ifndef __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DOOYA_485_H__
#define __RFIR_MODULE_DEVICE_CURTAIN_DOOYA_DOOYA_485_H__

#include "Arduino.h"
#include "../../device.h"

namespace rfir {
    namespace module {
        namespace device {
            namespace curtain {
                class Dooya485 {
                public:
                    static const uint8_t MaxBufLen = 64;
                    class RegAddr {
                    public:
                        static const uint8_t StartCode = 0x55;
                        static const uint8_t ID_L = 0x00;
                        static const uint8_t ID_H = 0x01;
                        static const uint8_t MT_POS = 0x02; //当前位置（百分比）
                        static const uint8_t MT_Dir = 0x03; //电机默认方向
                        static const uint8_t MT_State = 0x05; //电机状态 0x00—STOP 0x01—OPEN 0x02—CLOSE 0x03—SETTING
                        static const uint8_t MT_Angel = 0x06;  //当前角度 0-180
                        static const uint8_t MT_AngelDir = 0x07;  //角度调整方向  0x00—默认方向  0x01—反方向
                        static const uint8_t MT_AngelFac = 0x08;  //角度系数 0-255
                        static const uint8_t MT_TravelState = 0x09;  //行程状态 Bit1 已设行程点1 Bit2 已设行程点2 Bit3 已设行程点3 Bit4 已设行程点4
                        static const uint8_t MT_JogMode = 0x0A;  //点动模式	0 无点动，1-点动 (点动距离跟角度系数相关)
                        static const uint8_t MT_PowerOnPrompt = 0x0C;  //上电提示	0: 无提示 1：只有声音 2：只有转动 3：转动加声音
                        static const uint8_t DV_Model = 0xE0; //0xe0-0xef	信息	供主机读写
                        static const uint8_t DV_Type = 0xF0; //设备类型	0x11 卷帘	0x12 百叶帘
                        static const uint8_t DV_ChannelNum  = 0xF1; //模块通道数	1-15
                        static const uint8_t APP_Version  = 0xFD; //软件版本	0-255
                        static const uint8_t PRO_Version  = 0xFE; //协议版本	0xA4
                    };

                    struct RegData {
                        uint8_t id_l = 0xFE;
                        uint8_t id_h = 0xFE;
                        uint8_t pos = 0x00; //当前位置（百分比）
                        uint8_t dir = 0x00; //电机默认方向
                        uint8_t state = 0x00; //电机状态 0x00—STOP 0x01—OPEN 0x02—CLOSE 0x03—SETTING
                        uint8_t angel = 0x00;  //当前角度 0-180
                        uint8_t angelDir = 0x00;  //角度调整方向  0x00—默认方向  0x01—反方向
                        uint8_t angelFac = 0x00;  //角度系数 0-255
                        uint8_t travelState = 0x0;  //行程状态 Bit1 已设行程点1 Bit2 已设行程点2 Bit3 已设行程点3 Bit4 已设行程点4
                        uint8_t jogMode = 0x00;  //点动模式	0 无点动，1-点动 (点动距离跟角度系数相关)
                        uint8_t powerOnPrompt = 0x00;  //上电提示	0: 无提示 1：只有声音 2：只有转动 3：转动加声音
                        uint8_t model[0x10] = {}; //0xe0-0xef	信息	供主机读写
                        uint8_t type = 0x00; //设备类型	0x11 卷帘	0x12 百叶帘
                        uint8_t channelNum  = 0x00; //模块通道数	1-15
                        uint8_t appVersion  = 0x00; //软件版本	0-255
                        uint8_t proVersion  = 0x00; //协议版本	0xA4
                    };
                public:
                    Dooya485(HardwareSerial& hwSerial, int dePin, int rePin);
                public:
                    HardwareSerial* hwSerial;
                    int rePin;
                    int dePin;
                    uint8_t rxBuf[MaxBufLen];
                    uint8_t txBuf[MaxBufLen];
                public:
                    void readMode();
                    void writeMode();
                
                public:
                    RegData regData;
                    bool inited = false;
                    bool init();
                    bool initReg();
                    bool initState();
                    bool initInfo();
                    void setId(uint8_t id_l, uint8_t id_h);
                    bool readReg(uint8_t regAddr, uint8_t len);   

                    bool open();
                    bool close();
                    bool stop();

                    bool startRecvData(uint8_t* rx_buf, uint8_t len, unsigned long timeout_ms);
                    
                    bool startSendData(uint8_t* tx_buf, uint8_t len);

                    bool sendCode(uint8_t* tx_buf, uint8_t len);
                    bool recvCode(uint8_t* rx_buf, uint8_t& len, unsigned long timeout_ms = 200);
                    bool sendCodeAndRecv(uint8_t* tx_buf, uint8_t tx_len, uint8_t* rx_buf, uint8_t& rx_len, unsigned long timeout_ms = 200);

                    void dumpReg();
                    void dumpRxBuf(int len = MaxBufLen);


                };
            }
        }
    }
}
                

#endif                
