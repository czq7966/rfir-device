#include "rs485.h"
#include "../../../util/crc.h"


void rfir::module::device::RS::RS485::start(void * p) {
    Device::start(p);
    this->hwSerial = &Serial;
    this->dePin = 5;
    this->rePin = 5;
}

void rfir::module::device::RS::RS485::loop() {
    Device::loop();
}

rfir::module::ttl::Config::Device* rfir::module::device::RS::RS485::init() {
    pinMode(dePin, OUTPUT);
    pinMode(rePin, OUTPUT);   
    readMode();    

   this->name = "rs485";
    //创建设备
    auto ds = &rfir::RFIR::Config->devices;
    auto d = ds->newDevice(name);
    if (!d) return 0;

    d->packet.sniff.params.bufSize = 8;

    return d;   
}

void rfir::module::device::RS::RS485::readMode(){
    digitalWrite(dePin, 0);
    digitalWrite(rePin, 0);
};

void rfir::module::device::RS::RS485::writeMode(){
    digitalWrite(dePin, 1);
    digitalWrite(rePin, 1);
};


bool rfir::module::device::RS::RS485::onCmd_set(neb::CJsonObject* pld) {
    std::string code;
    pld->Get("code", code);
    auto nbytes = rfir::util::Util::StringToBytes(code, this->txBuf) / 8;
    
    if (nbytes > 0) {
        uint8_t len = 0;
        return sendCodeAndRecv(this->txBuf, nbytes, this->rxBuf, len);
    }
    
    return 0;
}

bool rfir::module::device::RS::RS485::onCmd_get(neb::CJsonObject* pld) {
    std::string code;
    pld->Get("code", code);

    auto nbytes = rfir::util::Util::StringToBytes(code, this->txBuf) / 8;
    if (nbytes > 0) {
        uint8_t len = 0;
        if(sendCodeAndRecv(this->txBuf, nbytes, this->rxBuf, len)) {
            code = rfir::util::Util::BytesToHexString(this->rxBuf, len);
            pld->ReplaceAdd("code", code);
        } else {            
            code = rfir::util::Util::BytesToHexString(this->rxBuf, len);
            pld->ReplaceAdd("errCode", code);
        } 
    }
    
    return 0;
}



bool rfir::module::device::RS::RS485::sendCode(uint8_t* tx_buf, uint8_t len) {
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

bool rfir::module::device::RS::RS485::recvCode(uint8_t* rx_buf, uint8_t& len, unsigned long timeout_ms) {
    bool result = false;
    readMode();

    static unsigned long Start_Recv_Code_time = 0;

    int c = -1;
    uint8_t idx = 0;
    Start_Recv_Code_time = millis();
    while ((millis() - Start_Recv_Code_time <= timeout_ms)) {
        if (this->hwSerial->available()) {
            c = this->hwSerial->read();
            if (c >=0) {
                rx_buf[idx] = (char)c;
                idx++;
                Start_Recv_Code_time = millis();
            }
        }
    }
    len = idx;
    
    if (len > 2) {
        uint8_t crcLow;
        uint8_t crcHigh;
        rfir::util::Crc::Get_CRC16(rx_buf, len - 2, crcLow, crcHigh);
        result = (rx_buf[len - 1] == crcHigh && rx_buf[len - 2] == crcLow);
    }    

    return result;

}


bool rfir::module::device::RS::RS485::sendCodeAndRecv(uint8_t* tx_buf, uint8_t tx_len, uint8_t* rx_buf, uint8_t& rx_len, unsigned long timeout_ms) {
    if (sendCode(tx_buf, tx_len)) {
        return recvCode(rx_buf, rx_len, timeout_ms);
    }

    return 0;
}
