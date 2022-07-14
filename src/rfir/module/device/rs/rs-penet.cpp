#include "rs-penet.h"
#include "rfir/util/base64.h"
#include "../networking.h"


void rfir::module::device::RS::RSPenet::init() {
    initSerial();
    this->name = "RSPenet";
}

void rfir::module::device::RS::RSPenet::initSerial() {
#ifdef COSerial       
    this->hwSerial = &COSerial;
    this->hwSerial->end();
    delay(100);    
    #ifdef ESP8266                    
        this->hwSerial->begin(CO_SERIAL_BAUD, SerialConfig(CO_SERIAL_CONFIG));
    #else
        this->hwSerial->begin(CO_SERIAL_BAUD, CO_SERIAL_CONFIG);
    #endif   
#else 
    this->hwSerial = &Serial;      
#endif

}

void rfir::module::device::RS::RSPenet::loop() {
    doEvt_penet();
}


int rfir::module::device::RS::RSPenet::rsRead(char buffer[], int offset) {
	if (hwSerial->available()) {
		int length = 0 + offset;
		while (hwSerial->available()) {
			buffer[length++] = (char)hwSerial->read();
		}
        return length - offset;
	}    
    return 0;
}

bool rfir::module::device::RS::RSPenet::rsWrite(char buffer[], size_t length, int offset) {
    size_t s = 0;
    while (s < length) {
        s = s + hwSerial->write(buffer + offset, length);
    }

    return s == length;
}


std::string  rfir::module::device::RS::RSPenet::rsReadBase64(char data[]) {
    int size = rsRead(data);
    if (size > 0) {
        int len = Base64.encodedLength(size);
        char encoded[len];
        Base64.encode(encoded, data, size);
        return encoded;
    }
    return "";
}

bool rfir::module::device::RS::RSPenet::rsWriteBase64(char data[], size_t size) {
	int len = Base64.decodedLength((char*)data, size);
	char decoded[len];
	Base64.decode(decoded, (char*)data, size);
    return rsWrite(decoded, len);
}

bool rfir::module::device::RS::RSPenet::doEvt_penet(){
    if (hwSerial->available() && GNetworking.status.connected && GNetworking.status.handshaked) {
        neb::CJsonObject pld;
        std::string code = rsReadBase64(rsBuffer);
        if (code.length() > 0) {
            pld.Add("raw", code);
            events.onEvtPenet.emit(&pld);
            return 1;
        } 
    }
    return 0;
};

int rfir::module::device::RS::RSPenet::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::RS::RSPenet::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::RS::RSPenet::onSvc_penet(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){    
    DEBUGER.printf("rfir::module::device::RS::RSPenet::onSvc_penet  \r\n");
    std::string code;
    if (pld && pld->Get("raw", code) ) {
        return rsWriteBase64((char*)code.c_str(), code.length());
    }    

    return 0;
};

