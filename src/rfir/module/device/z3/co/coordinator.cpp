#include "coordinator.h"
#include "rfir/util/base64.h"
#include "../../networking.h"


void rfir::module::device::z3::co::Coordinator::init() {
    this->name = "Z3Coordinator";
}



rfir::module::device::z3::co::Coordinator::Coordinator() {
    COSerial.end();
#ifdef CO_SERIAL_BAUD
    COSerial.begin(CO_SERIAL_BAUD, CO_SERIAL_CONFIG);
#else
    COSerial.begin(115200);
#endif   

    Serial.println(rfir::util::Util::GetChipId(CHIP_ID_PREFIX).c_str());
}

rfir::module::device::z3::co::Coordinator::~Coordinator() {

}

void rfir::module::device::z3::co::Coordinator::start(void * p) {
    Device::start(p);
}

void rfir::module::device::z3::co::Coordinator::loop() {
    doEvt_penet();
}

int rfir::module::device::z3::co::Coordinator::coRead(char buffer[]) {
	if (COSerial.available()) {
		int length = 0;
		while (COSerial.available()) {
			buffer[length++] = (char)COSerial.read();
		}
        return length;
	}    
    return 0;
}

bool rfir::module::device::z3::co::Coordinator::coWrite(char buffer[], size_t length) {
    size_t s = 0;
    while (s < length) {
        s = s + COSerial.write(buffer, length);
    }

    return s == length;
}


std::string  rfir::module::device::z3::co::Coordinator::coReadBase64(char data[]) {
    int size = coRead(data);
    if (size > 0) {
        int len = Base64.encodedLength(size);
        char encoded[len];
        Base64.encode(encoded, data, size);
        return encoded;
    }
    return "";
}

bool rfir::module::device::z3::co::Coordinator::coWriteBase64(char data[], size_t size) {
	int len = Base64.decodedLength((char*)data, size);
	char decoded[len];
	Base64.decode(decoded, (char*)data, size);
    return coWrite(decoded, len);
}

bool rfir::module::device::z3::co::Coordinator::doEvt_penet(){
    if (COSerial.available() && GNetworking.status.connected && GNetworking.status.handshaked) {
        neb::CJsonObject pld;
        std::string code = coReadBase64(coBuffer);
        if (code.length() > 0) {
            pld.Add("raw", code);
            events.onEvtPenet.emit(&pld);
            return 1;
        } 
    }
    return 0;
};

int rfir::module::device::z3::co::Coordinator::onSvc_get(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::z3::co::Coordinator::onSvc_set(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::z3::co::Coordinator::onSvc_penet(neb::CJsonObject* pld, cmds::cmd::CmdBase* cmd){    
    DEBUGER.printf("rfir::module::device::z3::co::Coordinator::onSvc_penet  \r\n");
    std::string code;
    if (pld && pld->Get("raw", code) ) {
        return coWriteBase64((char*)code.c_str(), code.length());
    }    

    return 0;
};

