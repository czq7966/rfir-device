#include "rfir-device.h"
#include "rfir/util/interrupt.h"
#include "service/cmds/cmd.h"



rfir::module::device::RFIRDevice::RFIRDevice() {
    sniffer = &GTTLSniffer;
    decoder = &GTTLDecoder;
    encoder = &GTTLEncoder;
    sender  = &GTTLSender;

    sniffer->events.onSniffed.add(this, std::bind(&RFIRDevice::onSniffed, this, std::placeholders::_1, std::placeholders::_2));
    decoder->events.onDecoded.add(this, std::bind(&RFIRDevice::onDecoded, this, std::placeholders::_1, std::placeholders::_2));
    encoder->events.onEncoded.add(this, std::bind(&RFIRDevice::onEncoded, this, std::placeholders::_1, std::placeholders::_2));
    sender->events.onSended.add(this, std::bind(&RFIRDevice::onSended, this, std::placeholders::_1, std::placeholders::_2));
}

rfir::module::device::RFIRDevice::~RFIRDevice() {
    sniffer->events.onSniffed.remove(this);
    decoder->events.onDecoded.remove(this);
    encoder->events.onEncoded.remove(this);
    sender->events.onSended.remove(this);
};

bool rfir::module::device::RFIRDevice::setConfig(const char* context){
    std::string rawStr;
    neb::CJsonObject json;
    if (json.Parse(context) && json.Get("raw", rawStr)) {
        setRaw(rawStr.c_str());
    }
    return false;
};

bool rfir::module::device::RFIRDevice::getConfig(std::string& context){
    auto rawStr = getRaw();
    neb::CJsonObject json;
    json.ReplaceAdd("raw", rawStr);
    context = json.ToString();
    return true;
}; 


void rfir::module::device::RFIRDevice::start(void * p) {
    init();
    loadConfig();
    sniffer->start();
    sniffer->startSniff();    
};

void rfir::module::device::RFIRDevice::loop() {
    sniffer->loop();

    Device::loop();
};

void* rfir::module::device::RFIRDevice::onSniffed(void* arg, void* p){
    auto data = (::rfir::module::ttl::SnifferV2::Data*)p;
    DEBUGER.println(data->toString().c_str());
    decoder->decode(data->deltas);
    return 0;
};

void* rfir::module::device::RFIRDevice::onDecoded(void* arg, void* p){
    auto decodeResults = (std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>*)p;

    std::string bitStr, hexStr;
    for (auto it = decodeResults->begin(); it != decodeResults->end(); it++)
    {
        auto decodeResult = *it;     
        auto bStr = decodeResult.toBitString();
        auto hStr = decodeResult.toHexString();

        hexStr = hexStr + (hexStr == "" ? hStr : "," + hStr);
        bitStr = bitStr + (bitStr == "" ? bStr : "," + bStr);
    }

    DEBUGER.printf("rfir::module::device::RFIRDevice::onDecoded: %s  %s \r\n", bitStr.c_str(), hexStr.c_str());  
    if (onSvc_decoded(*decodeResults)) {
        ::service::cmds::Cmd::OnSvc_get(0, "IR Change");
    };
    return 0;
};

void* rfir::module::device::RFIRDevice::onEncoded(void* arg, void* p){
    // DEBUGER.println("rfir::module::device::RFIRDevice::onEncoded");    
    // OnEncoded = true;
    // auto data = (std::list<uint16_t>*)p;
    // DEBUGER.println(encoder->toString(*data).c_str());

    // decoder->decode(*data);
    return 0;
};

void* rfir::module::device::RFIRDevice::onSended(void* arg, void* p){
    return 0;
};

std::string rfir::module::device::RFIRDevice::getRaw(){
    int count = 0;
    auto raw = getRaw(count);
    return "0x" + rfir::util::Util::BytesToHexString(raw, count);
};                 
bool rfir::module::device::RFIRDevice::setRaw(const char* rawStr){
    int count = 0;
    auto raw = getRaw(count);
    uint8_t bytes[count];
    memcpy(bytes, raw, count);
    rfir::util::Util::StringToBytes(rawStr, bytes);
    return setRaw(bytes);
};

bool rfir::module::device::RFIRDevice::setRaw(uint8_t* raw){
    return 0;
};
uint8_t* rfir::module::device::RFIRDevice::getRaw(int& count){
    return 0;
};     

bool rfir::module::device::RFIRDevice::sendRaw() {
    std::list<uint16_t> encodeRaw;
    if (getEncodeRaw(encodeRaw)) {
        GInterrupt.stop();        
        sender->sendRaw(encodeRaw);
        GInterrupt.start();
    }
    return false;

};

bool rfir::module::device::RFIRDevice::getEncodeRaw(std::list<uint16_t>& result){
    return 0;
};



bool rfir::module::device::RFIRDevice::onSvc_get(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) {
    return false;

}; 
bool rfir::module::device::RFIRDevice::onSvc_set(neb::CJsonObject* pld, ::cmds::cmd::CmdBase* cmd) {
    
    return false;
};

bool rfir::module::device::RFIRDevice::onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>& p) {

    return false;
};