#include "rfir-penet.h"
#include "rfir/util/base64.h"
#include "service/cmds/cmd.h"


void rfir::module::device::rfir::RFIRPenet::init() {
    RFIRDevice::init();

// #ifdef ESP8266
//     sniffer->params.pin = 14;
//     sender->params.pin = 4;
// #else     
//     sniffer->params.pin = 15;
//     sender->params.pin = 23;
// #endif  
    sniffer->name = Config.props.dev_model;
    sniffer->params.maxCount = 512;
    sender->params.repeat = 0;

    this->name = Config.props.dev_model;
}

bool rfir::module::device::rfir::RFIRPenet::loadConfig() {
    neb::CJsonObject config, device;
    Config.loadFromFile(config);
    if (config.Get("device", device))
        setConfig(&device);
    return 0;
}

bool rfir::module::device::rfir::RFIRPenet::saveConfig() {
    return 0;
}


bool rfir::module::device::rfir::RFIRPenet::setConfig(neb::CJsonObject* config) {
    neb::CJsonObject node, params;
    if (config->Get("sniff", node) && node.Get("params", params)) {
        sniffer->stopSniff();
        int enabled = sniffer->params.enabled;  params.Get("enabled", enabled); sniffer->params.enabled = enabled;
        int pin = sniffer->params.pin;  params.Get("pin", pin); sniffer->params.pin = pin;
        int mode = sniffer->params.mode;  params.Get("mode", mode); sniffer->params.mode = mode;
        int inverted = sniffer->params.inverted;  params.Get("inverted", inverted); sniffer->params.inverted = inverted;
        int minCount = sniffer->params.minCount;  params.Get("minCount", minCount); sniffer->params.minCount = minCount;
        int maxCount = sniffer->params.maxCount;  params.Get("maxCount", maxCount); sniffer->params.maxCount = maxCount;
        int minDelta = sniffer->params.minDelta;  params.Get("minDelta", minDelta); sniffer->params.minDelta = minDelta;
        
        sniffer->startSniff();
    }

    if (config->Get("send", node) && node.Get("params", params)) {
        int enabled = sender->params.enabled;  params.Get("enabled", enabled); sender->params.enabled = enabled;
        int pin = sender->params.pin;  params.Get("pin", pin); sender->params.pin = pin;
        int inverted = sender->params.inverted;  params.Get("inverted", inverted); sender->params.inverted = inverted;
        int modulation = sender->params.modulation;  params.Get("modulation", modulation); sender->params.modulation = modulation;
        int repeat = sender->params.repeat;  params.Get("repeat", repeat); sender->params.repeat = repeat;
        int frequency = sender->params.frequency;  params.Get("frequency", frequency); sender->params.frequency = frequency;
        int dutycycle = sender->params.dutycycle;  params.Get("dutycycle", dutycycle); sender->params.dutycycle = dutycycle;
    
    }

    return 0;
};

void* rfir::module::device::rfir::RFIRPenet::onSniffed(void* arg, void* p){
    m_sniffed_data = (::rfir::module::ttl::SnifferV2::Data*)p;
    DEBUGER.println(m_sniffed_data->toString().c_str());
    ::service::cmds::Cmd::OnSvc_get(0, "On Sniffed");
    m_sniffed_data = 0;
    return 0;
};

void* rfir::module::device::rfir::RFIRPenet::onSended(void* arg, void* p){
    return 0;
};

bool rfir::module::device::rfir::RFIRPenet::rfirWrite(char buffer[], size_t length, int offset) {
    this->sender->sendRaw(buffer + offset, length );
    return true;
}

bool rfir::module::device::rfir::RFIRPenet::rfirWrite(uint16_t buffer[], size_t length, int offset) {
    this->sender->sendRaw(buffer + offset, length);
    return true;
}

bool rfir::module::device::rfir::RFIRPenet::rfirWriteBase64(char data[], size_t size) {
	int len = Base64.decodedLength((char*)data, size);
	char decoded[len];
	Base64.decode(decoded, (char*)data, size);
    return rfirWrite((uint16_t*)decoded, len / 2);
}

int rfir::module::device::rfir::RFIRPenet::onSvc_get(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd){
    if (m_sniffed_data && m_sniffed_data->deltas.size() > 0) {
        int size = m_sniffed_data->deltas.size() + 1;
        std::string raw;
        {
            char encoded[Base64.encodedLength(size * 2)];
            {
                uint16_t buffer[size];
                size = m_sniffed_data->toBuffer(buffer);
                Base64.encode(encoded, (char*)buffer, size * 2);
            }
            raw = encoded;
        }
        pld->ReplaceAdd("raw", raw);
        return 1;

    }
    return 0;
}; 

int rfir::module::device::rfir::RFIRPenet::onSvc_set(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd){
    return onSvc_penet(pld, cmd);
}; 

int rfir::module::device::rfir::RFIRPenet::onSvc_config(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd){
    neb::CJsonObject config, device;
    if (pld->Get("config", config) && config.Get("device", device))
        return setConfig(&device);
    return 0;
}; 

int rfir::module::device::rfir::RFIRPenet::onSvc_penet(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd){    
    std::string code;
    if (pld && pld->Get("raw", code) ) {
        DEBUGER.printf("rfir::module::device::RS::RSPenet::onSvc_penet %s \r\n", code.c_str());
        return rfirWriteBase64((char*)code.c_str(), code.length());
    }  

    if (pld && pld->Get("rawStr", code) ) {
        DEBUGER.printf("rfir::module::device::RS::RSPenet::onSvc_penet %s \r\n", code.c_str());
        return rfirWrite((char*)code.c_str(), code.length());
    }    

    return 0;
};

