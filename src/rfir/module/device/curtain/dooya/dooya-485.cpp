#include "dooya-485.h"
#include "util/crc.h"

rfir::module::device::curtain::Dooya485::Dooya485(HardwareSerial& hwSerial, int dePin, int rePin) :
  hwSerial(&hwSerial),
  dePin(dePin),
  rePin(rePin)
{
    setId(0xFE, 0xFE);
    pinMode(dePin, OUTPUT);
    pinMode(rePin, OUTPUT);   
    readMode();
}

void rfir::module::device::curtain::Dooya485::readMode(){
    digitalWrite(dePin, 0);
    digitalWrite(rePin, 0);
};

void rfir::module::device::curtain::Dooya485::writeMode(){
    digitalWrite(dePin, 1);
    digitalWrite(rePin, 1);
};

bool rfir::module::device::curtain::Dooya485::init(){
    if (!inited) {        
        inited = initReg();   
    }

    return inited;
    
    
};

bool rfir::module::device::curtain::Dooya485::initReg(){
    // return initInfo();
    return initState() && initInfo();
};

bool rfir::module::device::curtain::Dooya485::initState(){
    if (readReg(0x00, 0x10)) {
        regData.id_l    = rxBuf[RegAddr::ID_L];
        regData.id_l    = rxBuf[RegAddr::ID_H];
        regData.pos     = rxBuf[RegAddr::MT_POS];
        regData.dir     = rxBuf[RegAddr::MT_Dir];
        regData.state   = rxBuf[RegAddr::MT_State];
        regData.angel   = rxBuf[RegAddr::MT_Angel];
        regData.angelDir = rxBuf[RegAddr::MT_AngelDir];  //角度调整方向  0x00—默认方向  0x01—反方向
        regData.angelFac = rxBuf[RegAddr::MT_AngelFac];  //角度系数 0-255
        regData.travelState = rxBuf[RegAddr::MT_TravelState];  //行程状态 Bit1 已设行程点1 Bit2 已设行程点2 Bit3 已设行程点3 Bit4 已设行程点4
        regData.jogMode = rxBuf[RegAddr::MT_JogMode];  //点动模式	0 无点动，1-点动 (点动距离跟角度系数相关)
        regData.powerOnPrompt = rxBuf[RegAddr::MT_PowerOnPrompt];  //上电提示	0: 无提示 1：只有声音 2：只有转动 3：转动加声音
        dumpReg();
        return 1;
    }

    return 0;
};

bool rfir::module::device::curtain::Dooya485::initInfo(){
    if (readReg(0xe0, 0x10)) {
        memcpy(regData.model, rxBuf, 0x10);
        dumpReg();
    }  else return 0;

    if (readReg(0xf0, 0x10)) {
        regData.type = rxBuf[0x00];
        regData.channelNum  = rxBuf[0x01];
        regData.appVersion  = rxBuf[0x0d];
        regData.proVersion  = rxBuf[0x0e];        
        dumpReg();
    }  else return 0;

    return 1;
};

void rfir::module::device::curtain::Dooya485::setId(uint8_t id_l, uint8_t id_h){
    regData.id_l = id_l;
    regData.id_h = id_h;
};

bool rfir::module::device::curtain::Dooya485::readReg(uint8_t regAddr, uint8_t len){
    int idx = 0;
    txBuf[idx++] = 0x55;
    txBuf[idx++] = regData.id_l;
    txBuf[idx++] = regData.id_h;
    txBuf[idx++] = 0x01;
    txBuf[idx++] = regAddr;
    txBuf[idx++] = len;
    curtain::dooya::util::Crc::Get_CRC16(txBuf, idx, txBuf[idx++], txBuf[idx++]);

    delayMicroseconds(200 * 1000);
    if (startSendData(txBuf, idx)) {
        if (startRecvData(rxBuf, idx + len - 1, 100)) {
            memcpy(rxBuf, rxBuf + 5, len);
            return 1;
        }
    }
    return 0;
};       


bool rfir::module::device::curtain::Dooya485::open() {
    int idx = 0;
    txBuf[idx++] = 0x55;
    txBuf[idx++] = regData.id_l;
    txBuf[idx++] = regData.id_h;
    txBuf[idx++] = 0x03;
    txBuf[idx++] = 0x01;
    curtain::dooya::util::Crc::Get_CRC16(txBuf, idx, txBuf[idx++], txBuf[idx++]);

    if (startSendData(txBuf, idx)) {
        return startRecvData(rxBuf, idx , 100);
    }
    return 0;
}

bool rfir::module::device::curtain::Dooya485::close() {
    int idx = 0;
    txBuf[idx++] = 0x55;
    txBuf[idx++] = regData.id_l;
    txBuf[idx++] = regData.id_h;
    txBuf[idx++] = 0x03;
    txBuf[idx++] = 0x02;
    curtain::dooya::util::Crc::Get_CRC16(txBuf, idx, txBuf[idx++], txBuf[idx++]);

    if (startSendData(txBuf, idx)) {
        return startRecvData(rxBuf, idx , 100);
    }
    return 0;
}

bool rfir::module::device::curtain::Dooya485::stop() {
    int idx = 0;
    txBuf[idx++] = 0x55;
    txBuf[idx++] = regData.id_l;
    txBuf[idx++] = regData.id_h;
    txBuf[idx++] = 0x03;
    txBuf[idx++] = 0x03;
    curtain::dooya::util::Crc::Get_CRC16(txBuf, idx, txBuf[idx++], txBuf[idx++]);

    if (startSendData(txBuf, idx)) {
        return startRecvData(rxBuf, idx , 100);
    }
    return 0;
}

bool rfir::module::device::curtain::Dooya485::startRecvData(uint8_t* rx_buf, uint8_t len, unsigned long timeout_ms) {
    bool result = false;
    readMode();

    static unsigned long Dooya485_Start_Recv_Data_time = 0;

    int c = -1;
    uint8_t idx = 0;
    Dooya485_Start_Recv_Data_time = millis();
    while ((millis() - Dooya485_Start_Recv_Data_time <= timeout_ms)) {
        if (this->hwSerial->available()) {
            c = this->hwSerial->read();
            if (c >=0) {
                rx_buf[idx] = (char)c;
                idx++;
                Dooya485_Start_Recv_Data_time = millis();
            }
        }
    }
    

    dumpRxBuf(idx);

    if (idx >= len) {
        uint8_t crcLow;
        uint8_t crcHigh;
        curtain::dooya::util::Crc::Get_CRC16(rx_buf, len - 2, crcLow, crcHigh);
        result = (rx_buf[len - 1] == crcHigh && rx_buf[len - 2] == crcLow);
    }    

    return result;
}

bool rfir::module::device::curtain::Dooya485::startSendData(uint8_t* tx_buf,uint8_t len) {
    return sendCode(tx_buf, len);
}


bool rfir::module::device::curtain::Dooya485::sendCode(uint8_t* tx_buf, uint8_t len) {
    // DEBUGER.println(rfir::util::Util::BytesToHexString(tx_buf, len).c_str());
    this->hwSerial->flush();
    writeMode();    

	for(int i = 0; i < len; i++)
	{
        this->hwSerial->write(tx_buf[i]);		
	}    

    this->hwSerial->flush();

    readMode();   
    return 1;
}

bool rfir::module::device::curtain::Dooya485::recvCode(uint8_t* rx_buf, uint8_t& len, unsigned long timeout_ms) {
    bool result = false;
    readMode();

    static unsigned long Dooya485_Start_Recv_Code_time = 0;

    int c = -1;
    uint8_t idx = 0;
    Dooya485_Start_Recv_Code_time = millis();
    while ((millis() - Dooya485_Start_Recv_Code_time <= timeout_ms)) {
        if (this->hwSerial->available()) {
            c = this->hwSerial->read();
            if (c >=0) {
                rx_buf[idx] = (char)c;
                idx++;
                Dooya485_Start_Recv_Code_time = millis();
            }
        }
    }
    len = idx;
    
    if (len > 2) {
        uint8_t crcLow;
        uint8_t crcHigh;
        curtain::dooya::util::Crc::Get_CRC16(rx_buf, len - 2, crcLow, crcHigh);
        result = (rx_buf[len - 1] == crcHigh && rx_buf[len - 2] == crcLow);
    }    

    return result;

}


bool rfir::module::device::curtain::Dooya485::sendCodeAndRecv(uint8_t* tx_buf, uint8_t tx_len, uint8_t* rx_buf, uint8_t& rx_len, unsigned long timeout_ms) {
    if (sendCode(tx_buf, tx_len)) {
        return recvCode(rx_buf, rx_len, timeout_ms);
    }

    return 0;
}

void rfir::module::device::curtain::Dooya485::dumpReg() {
    DEBUGER.printf("id_l = %x \r\n", regData.id_l );
    DEBUGER.printf("id_h = %x \r\n", regData.id_h );
    DEBUGER.printf("pos = %x \r\n", regData.pos );
    DEBUGER.printf("dir = %x \r\n", regData.dir );
    DEBUGER.printf("state = %x \r\n", regData.state );
    DEBUGER.printf("angel = %x \r\n", regData.angel );
    DEBUGER.printf("angelDir = %x \r\n", regData.angelDir );
    DEBUGER.printf("angelFac = %x \r\n", regData.angelFac );
    DEBUGER.printf("travelState = %x \r\n", regData.travelState );
    DEBUGER.printf("jogMode = %x \r\n", regData.jogMode );
    DEBUGER.printf("powerOnPrompt = %x \r\n", regData.powerOnPrompt );
    DEBUGER.printf("model = %s \r\n", regData.model );
    DEBUGER.printf("type = %x \r\n", regData.type );
    DEBUGER.printf("channelNum = %x \r\n", regData.channelNum );
    DEBUGER.printf("appVersion = %x \r\n", regData.appVersion );
    DEBUGER.printf("proVersion = %x \r\n", regData.proVersion );
}

void rfir::module::device::curtain::Dooya485::dumpRxBuf(int len) {
    for (size_t i = 0; i < len; i++)
    {
        DEBUGER.write(rxBuf[i]);
    }
}