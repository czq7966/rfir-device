#include "config.h"

String rfir::module::ttl::Config::ConfigFilename = "/rfir-config.json";

rfir::module::ttl::Config::Config() {
    //init();
}

rfir::module::ttl::Config::~Config() {
    uninit();
}

bool rfir::module::ttl::Config::init() {
    if (!loadConfig()) {
        Serial.println("error::加载配置文件失败");
        String str = TTL_DEFAULT_CONFIG_STR;
            str.replace(" ", "");
            str.replace("   ", "");
            str.replace("\r", "");
            str.replace("\n", "");
            str.replace("\r\n", "");
        if (!changeConfig(str, true)) {
            Serial.println("error::加载默认配置内容失败");
            Serial.println(str.c_str());
            return false;
        }       
    }    
    return true;
}

bool rfir::module::ttl::Config::init(neb::CJsonObject* jCfg) {
    initDevices(jCfg, &this->devices);

    if (jCfg != &jDevices) {
        jDevices = *jCfg;
    }
    
    return true;
}

bool rfir::module::ttl::Config::initDevices(neb::CJsonObject* jdevices, Devices* devices) {
    uninitDevices(devices);
    std::string name;
    int  count = 0;
    while(jdevices->GetKey(name)) {
        count++;
    }

    devices->device = new Device[count];
    devices->count = count;

    int idx = 0;
    while(jdevices->GetKey(name)) {
        Device* device = devices->device + idx;
        device->name = name;
        neb::CJsonObject jdevice;
        if (jdevices->Get(name, jdevice)) {
            initDevice(&jdevice, devices->device + idx);
        }
        
        idx++;        
    }    

    return true;

}

bool rfir::module::ttl::Config::initDevice(neb::CJsonObject* jdevice, Device* device) {
    neb::CJsonObject jsniff, jdecode, jsend, jencode;
    if (jdevice->Get("sniff", jsniff)) {
        if (!initDeviceSniff(&jsniff, &(device->packet.sniff)))
            return false;
    }
    
    if (jdevice->Get("decode", jdecode)) {
        if (!initDeviceDecode(&jdecode, &(device->packet.decode)))
            return false;
    }
    if (jdevice->Get("send", jsend)) {
        if (!initDeviceSend(&jsend, &(device->packet.send)))
            return false;
    }
    if (jdevice->Get("encode", jencode)) {
        if (!initDeviceEncode(&jencode, &(device->packet.encode)))
            return false;
    }    
    
    return true;
}

bool rfir::module::ttl::Config::initDeviceSniff(neb::CJsonObject* jsniff, rfir::module::ttl::Sniffer::SniffParams* sniff) {
    neb::CJsonObject jp;
    if (!jsniff->Get("params", jp) || !Sniffer::parseParams(&jp, &(sniff->params))) {
        Serial.println("initDeviceSniff Failed");
        return false;        
    }
    return true;
}

bool rfir::module::ttl::Config::initDeviceDecode(neb::CJsonObject* jdecode, rfir::module::ttl::Decoder::DecodeParams* decode) {
    neb::CJsonObject jblocks;
    if (jdecode->Get("blocks", jblocks)) {
        if (jblocks.IsArray()) {
            int count = jblocks.GetArraySize();            
            decode->params = new Decoder::Params[count];
            decode->count = count;
            for (size_t i = 0; i < count; i++)
            {
                neb::CJsonObject bl;
                neb::CJsonObject jp;                
                if (!jblocks.Get(i, bl) || !bl.Get("params", jp) || !Decoder::parseParams(&jp, decode->params + i)) {
                    Serial.println("initDeviceDecode Failed");                
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

bool rfir::module::ttl::Config::initDeviceSend(neb::CJsonObject* jsend, rfir::module::ttl::Sender::SendParams* send) {
    neb::CJsonObject jp;
    if (!jsend->Get("params", jp) || !Sender::parseParams(&jp, &(send->params))) {
        Serial.println("initDeviceSend Failed");
        return false;        
    }
    return true;
}

bool rfir::module::ttl::Config::initDeviceEncode(neb::CJsonObject* jencode, rfir::module::ttl::Encoder::EncodeParams* encode) {
    neb::CJsonObject jblocks;
    if (jencode->Get("blocks", jblocks)) {
        if (jblocks.IsArray()) {
            int count = jblocks.GetArraySize();            
            encode->params = new Encoder::Params[count];
            encode->count = count;
            for (size_t i = 0; i < count; i++)
            {
                neb::CJsonObject bl;
                neb::CJsonObject jp;
                
                if (!jblocks.Get(i, bl) || !bl.Get("params", jp) || !Encoder::parseParams(&jp, encode->params + i)) {
                    Serial.println("initDeviceEncode Failed");                
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

void rfir::module::ttl::Config::uninit() {
    uninitDevices(&this->devices);
}

bool rfir::module::ttl::Config::uninitDevices(Devices* devices) {
    for (size_t i = 0; i < devices->count; i++)
    {
        uninitDevice(devices->device + i);
    }
    
    delete devices->device;
    devices->device = 0;
    devices->count = 0;
    return true;
}

bool rfir::module::ttl::Config::uninitDevice(Device* device) {
    uninitDeviceDecode(&(device->packet.decode));
    uninitDeviceEncode(&(device->packet.encode));
    return true;
}

bool rfir::module::ttl::Config::uninitDeviceDecode(rfir::module::ttl::Decoder::DecodeParams* decode) {
    delete decode->params;
    decode->params = 0;
    decode->count = 0;
    return 1;
}

bool rfir::module::ttl::Config::uninitDeviceEncode(rfir::module::ttl::Encoder::EncodeParams* encode) {
    delete encode->params;
    encode->params = 0;
    encode->count = 0;
    return 1;
}

bool rfir::module::ttl::Config::changeConfig(neb::CJsonObject& jCfg, bool save) {
    bool result = init(&jCfg);

    if (save) {
        rfir::util::TxtFile file(rfir::module::ttl::Config::ConfigFilename.c_str());
        std::string str = jCfg.ToString();
        int count = file.writeString(str.c_str(), str.length());
        Serial.println("写入文件: " + String(count));        
    }

    return result;
}

bool rfir::module::ttl::Config::changeConfig(String& strCfg, bool save) {
    neb::CJsonObject json;
    if (json.Parse(strCfg.c_str()))    
        return changeConfig(json, save);

    return false;
}

bool rfir::module::ttl::Config::changeConfigFromFile(String fn) {
    util::TxtFile file(fn.c_str());
    String str;
    if (file.readString(str))
        return changeConfig(str, false);
    return false; 
}

bool rfir::module::ttl::Config::loadConfig() {
    return changeConfigFromFile(rfir::module::ttl::Config::ConfigFilename);
}    