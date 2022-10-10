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
        this->hwSerial->begin(Config.props.co_serial_baud, SerialConfig(Config.props.co_serial_config));
    #else
        this->hwSerial->begin(Config.props.co_serial_baud., Config.props.co_serial_config);
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
    for(int i = 0; i < length; i++)
	{
        this->hwSerial->write(buffer[i + offset]);		
	}    

    return true;
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
    if (hwSerial->available() && GNetworking.status.connected) {
        DynamicJsonDocument doc(Config.props.mqtt_buffer_size);
        JsonObject pld = doc.to<JsonObject>();
        std::string code = rsReadBase64(rsBuffer);
        if (code.length() > 0) {
            pld["raw"] = code;
            events.onEvtPenet.emit(&pld);
            return 1;
        } 
    }
    return 0;
};

int rfir::module::device::RS::RSPenet::onSvc_get(JsonObject* pld, cmds::cmd::CmdBase* cmd){
    return 0;
}; 

int rfir::module::device::RS::RSPenet::onSvc_set(JsonObject* pld, cmds::cmd::CmdBase* cmd){
    onSvc_penet(pld, cmd);
    return -1;
}; 

int rfir::module::device::RS::RSPenet::onSvc_penet(JsonObject* pld, cmds::cmd::CmdBase* cmd){        
    if (pld && pld->containsKey("raw") ) {
        std::string code = (*pld)["raw"];
        GDebuger.printf("rfir::module::device::RS::RSPenet::onSvc_penet %s \r\n", code.c_str());
        auto result = rsWriteBase64((char*)code.c_str(), code.length());
            
        return result;
    }    

    return 0;
};

