#include "mcquay.h"

#ifdef RFIR_DEVICE_MCQUAY


rfir::module::ttl::Config::Device* rfir::service::device::ac::Mcquay::Init(std::string name) {
    auto ds = &rfir::RFIR::Config->devices;
    //创建设备
    auto d = ds->newDevice(name);
    if (!d) return 0;

    return d;
}


bool rfir::service::device::ac::Mcquay::OnCmd(const char* cmd) {
    return true;

}

bool rfir::service::device::ac::Mcquay::OnCmd(neb::CJsonObject* cmd) {
    return true;
}




uint16_t* rfir::service::device::ac::Mcquay::getEncodeRaw(uint16_t& count) {
    // auto raw = Device->getRaw();
    // auto str1 = rfir::util::Util::BytesToString(raw, 4);
    // uint8_t bits[1] = {0b010};
    // auto str2 = rfir::util::Util::BitsToString(bits, 3);
    // auto str3 = rfir::util::Util::BytesToString(raw + 3, 4);

    // String str = "[{'data': '%data1%'}, {'data': '%data2%'}, {'data': '%data3%'}]";
    // str.replace("'", "\"");
    // str.replace("%data1%", str1.c_str());
    // str.replace("%data2%", str2.c_str());
    // str.replace("%data3%", str3.c_str());
    
    // neb::CJsonObject blocks;
    // blocks.Parse(str.c_str());
    // return 0;


    // auto rfir = rfir::GetRfir("gree");
    // rfir->encoder->encode(&blocks);
    // auto encode = rfir->encoder->getEncodeResult();    
    // count = encode->count;
    // return encode->result;
    return 0;
}

#endif