#include "rs485.h"
#include "../../../util/crc.h"


void rfir::module::device::RS::RS485::init() {
#ifdef COSerial       
    this->hwSerial = &COSerial;
    neb::CJsonObject pld;
    pld.Add("baudRate", CO_SERIAL_BAUD);
    pld.Add("serialConfig", CO_SERIAL_CONFIG);
    onSvc_setBandRate(&pld);
#else 
    this->hwSerial = &Serial;      
#endif
    this->dePin = 5;
    this->rePin = 5;    
    pinMode(dePin, OUTPUT);
    pinMode(rePin, OUTPUT);   
    readMode();    
}


void rfir::module::device::RS::RS485::loop() {
    if (this->hwSerial->available()) {
        std::string reason = "Prop Change";
        this->events.onEvtPropsChange.emit((void*)reason.c_str());
    }
};

void rfir::module::device::RS::RS485::readMode(){
    digitalWrite(dePin, 0);
    digitalWrite(rePin, 0);
};

void rfir::module::device::RS::RS485::writeMode(){
    digitalWrite(dePin, 1);
    digitalWrite(rePin, 1);
};


bool rfir::module::device::RS::RS485::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    onSvc_setBandRate(pld);

    std::string code;
    pld->Get("code", code);
    auto nbytes = rfir::util::Util::StringToBytes(code, this->txBuf) / 8;
    
    if (nbytes > 0) {
        uint8_t len = 0;
        return sendCodeAndRecv(this->txBuf, nbytes, this->rxBuf, len, this->recvTimeoutMS);
    }
    
    return 0;
}

bool rfir::module::device::RS::RS485::onSvc_setBandRate(neb::CJsonObject* pld) {
    int baudRate, serialConfig;
    if (pld->Get("baudRate", baudRate)) {
        this->hwSerial->end();
        delay(100);
        if (pld->Get("serialConfig", serialConfig)) {   
            #ifdef ESP8266                    
                this->hwSerial->begin(baudRate, SerialConfig(serialConfig));
            #else
                this->hwSerial->begin(baudRate, serialConfig);
            #endif            
        } else {
            #ifdef SERIAL_CONFIG
                this->hwSerial->begin(baudRate, SERIAL_CONFIG);
            #else
                this->hwSerial->begin(baudRate);
            #endif
        }
        delay(100);
        return 1;
    }
    return 0;
}; 

bool rfir::module::device::RS::RS485::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd) {
    std::string code;
    pld->Get("code", code);

    auto nbytes = rfir::util::Util::StringToBytes(code, this->txBuf) / 8;
    if (nbytes > 0) {
        uint8_t len = 0;
        if(sendCodeAndRecv(this->txBuf, nbytes, this->rxBuf, len, this->recvTimeoutMS)) {
            code = rfir::util::Util::BytesToHexString(this->rxBuf, len);
            pld->ReplaceAdd("code", code);
            return 1;
        } else {            
            code = rfir::util::Util::BytesToHexString(this->rxBuf, len);
            pld->ReplaceAdd("errCode", code);
            return 0;
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

    int c = -1;
    uint8_t idx = 0;
    unsigned long startTime = millis();
    unsigned long entTime = startTime;

    while (this->hwSerial->available() || (entTime - startTime <= timeout_ms)) {
        if (this->hwSerial->available()) {
            c = this->hwSerial->read();
            if (c >=0) {
                rx_buf[idx] = (char)c;
                idx++;
                startTime = millis();
            }
        }
        entTime = millis();
    }

    len = idx;
    result = (len > 0) && checkSumCode(rx_buf, len);

    return result;
}


bool rfir::module::device::RS::RS485::sendCodeAndRecv(uint8_t* tx_buf, uint8_t tx_len, uint8_t* rx_buf, uint8_t& rx_len, unsigned long timeout_ms) {
    if (sendCode(tx_buf, tx_len)) {
        return recvCode(rx_buf, rx_len, timeout_ms);
    }

    return 0;
}


 bool rfir::module::device::RS::RS485::checkSumCode(uint8_t* buf, uint8_t len) {
    return true;
 };
