#include "coordinator.h"
#include "rfir/util/base64.h"


rfir::module::ttl::Config::Device* rfir::module::device::z3::co::Coordinator::init() {
    this->name = "Z3Coordinator";
    auto ds = &rfir::RFIR::Config->devices;
    //创建设备
    auto d = ds->newDevice(name);
    if (!d) return 0;

    //采码参数
    auto sp = &d->packet.sniff.params;
    sp->minCount = 50;
    sp->minDelta = 150;
    sp->maxDelta = 65535;
    sp->inverted = true;
    sp->mode = INPUT;
    sp->bufSize = 1;

    //发码参数
    d->packet.send.params.inverted = false;
    d->packet.send.params.repeat = 0;    
    d->packet.send.params.modulation = true;        

    //解码参数
    d->packet.decode.create(1);    
    auto dp = d->packet.decode.params;

    dp[0].tolerance = 20;
    dp[0].excess = 0;
    dp[0].atleast = true;                              
    dp[0].MSBfirst = false;
    dp[0].step = 2;
    dp[0].use_bits = false;
    
    dp[0].nbits = 8;
    dp[0].headermark = 0;
    dp[0].headerspace = 0;
    dp[0].onemark = 0;
    dp[0].onespace = 0;
    dp[0].zeromark = 0;
    dp[0].zerospace = 0;
    dp[0].footermark = 0;
    dp[0].footerspace = 0;
    dp[0].lastspace = 0;


    //编码参数
    d->packet.encode.clone(&d->packet.decode);

    
    return d;
}



rfir::module::device::z3::co::Coordinator::Coordinator() {
    COSerial.end();
#ifdef CO_SERIAL_BAUD
    COSerial.begin(CO_SERIAL_BAUD, CO_SERIAL_CONFIG);
#else
    COSerial.begin(115200);
#endif   
}

rfir::module::device::z3::co::Coordinator::~Coordinator() {

}

void rfir::module::device::z3::co::Coordinator::start(void * p) {

}

void rfir::module::device::z3::co::Coordinator::loop() {
    if (COSerial.available()) {
        emitChange("");
    }
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

bool rfir::module::device::z3::co::Coordinator::setRaw(uint8_t* raw) {
    return 0;
}

uint8_t* rfir::module::device::z3::co::Coordinator::getRaw(int& count) {
    return 0;
}

uint16_t* rfir::module::device::z3::co::Coordinator::getEncodeRaw(int& count) {
    return 0;

}

bool rfir::module::device::z3::co::Coordinator::onCmd_set(neb::CJsonObject* pld) {
    std::string code;
    if (pld && pld->Get("code", code) ) {
        return coWriteBase64((char*)code.c_str(), code.length());
    }    

    return 0;
}


bool rfir::module::device::z3::co::Coordinator::onCmd_get(neb::CJsonObject* pld) {
    std::string code = coReadBase64(coBuffer);
    if (code.length() > 0) {
        pld->Add("code", code);
        return 1;
    } 

    return 0;
}

bool rfir::module::device::z3::co::Coordinator::onCmd_decoded(rfir::module::ttl::Decoder::DecodeResults* data) {
    return 0;

}

void  rfir::module::device::z3::co::Coordinator::dump() {
    return;
}


void rfir::module::device::z3::co::Coordinator::doTimerReport(bool reset) {
    return;
}