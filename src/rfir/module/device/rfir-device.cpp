#include "rfir-device.h"



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

void rfir::module::device::RFIRDevice::start(void * p) {
    Device::start(p);    
    init();

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
    onSvc_decoded(decodeResults);    
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


bool rfir::module::device::RFIRDevice::loadRaw();      
bool rfir::module::device::RFIRDevice::saveRaw();   
bool rfir::module::device::RFIRDevice::setRaw(uint8_t* raw);
uint8_t* rfir::module::device::RFIRDevice::getRaw(int& count);                
bool rfir::module::device::RFIRDevice::getEncodeRaw(std::list<uint16_t>& result);



bool rfir::module::device::RFIRDevice::onSvc_get(neb::CJsonObject* pld) {
    return false;

}; 
bool rfir::module::device::RFIRDevice::onSvc_set(neb::CJsonObject* pld) {
    
    return false;
};

bool rfir::module::device::RFIRDevice::onSvc_decoded(std::vector<::rfir::module::ttl::DecoderV2::DecodeResult>* p) {

    return false;
};