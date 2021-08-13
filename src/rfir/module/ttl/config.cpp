#include "rfir.h"
#include "mem.h"

rfir::module::ttl::Config::Config(RFIR* rfir) {
    this->rfir = rfir;
}

rfir::module::ttl::Config::~Config() {
    this->devices.free();
}

bool rfir::module::ttl::Config::init(std::string fn) {
    if (loadFromFile(fn)) {
        Serial.println("加载文件配置成功");
    } else {
        Serial.println(("加载文件配置失败：" + fn).c_str());

        if (loadFromString(TTL_DEFAULT_CONFIG_STR)) {
            Serial.println("加载默认配置成功");
        }
        else {
            Serial.println("加载默认配置失败");        
            return false;
        }
    }    

    return true;
}


void rfir::module::ttl::Config::uninit() {
    this->devices.free();
    //uninitDevices(&this->devices);
}

rfir::module::ttl::Config::Device* rfir::module::ttl::Config::getDevice(std::string name) {
    return this->devices.getDevice(name);
}

bool rfir::module::ttl::Config::loadFromFile(std::string fn) {
    util::TxtFile file(fn.c_str());
    std::string str;
    if (file.readString(str))
        return loadFromString(str.c_str());

    return 0;
}

bool rfir::module::ttl::Config::loadFromJson(neb::CJsonObject * jdevices) {
    return this->devices.parseFromJson(jdevices);
}

bool rfir::module::ttl::Config::loadFromString(const char* str) {
    neb::CJsonObject json;
    if (json.Parse(str)) {
        return loadFromJson(&json);
    }

    return 0;
}

void rfir::module::ttl::Config::Packet::free() {
    this->decode.free();
    this->encode.free();
}

bool rfir::module::ttl::Config::Packet::parseFromJson(neb::CJsonObject* jp) {
    if (jp) {
        this->free();
        neb::CJsonObject jsniff, jsend, jdecode, jencode;
        if (jp->Get("sniff", jsniff)) {
            this->sniff.parseFromJson(&jsniff);
        }
        if (jp->Get("send", jsend))
            this->send.parseFromJson(&jsend);
        if (jp->Get("decode", jdecode)) {
            this->decode.parseFromJson(&jdecode);            
            this->encode.clone(&this->decode);
        }
        if (jp->Get("encode", jencode))
            this->encode.parseFromJson(&jencode);    
        // this->sniff.params.bufSize = 512;     
        return true;   
    }
    return false;

}

bool rfir::module::ttl::Config::Packet::clone(Packet* p) {
    free();

    this->sniff.clone(&p->sniff);
    this->send.clone(&p->send);
    this->decode.clone(&p->decode);
    this->encode.clone(&p->encode);
    return 1;
}

void rfir::module::ttl::Config::Device::free() {
    delete this->name;
    this->name = 0;
    this->packet.free();
    
}


bool rfir::module::ttl::Config::Device::parseFromJson(neb::CJsonObject* jp, const char* name) {    
    this->free();
    this->name = new std::string();
    if (name) (*this->name) = name;
    return this->packet.parseFromJson(jp);
}

bool rfir::module::ttl::Config::Device::clone(Device* d) {
    this->free();
    this->name = new std::string(d->getName());
    return this->packet.clone(&d->packet);
    
}

std::string rfir::module::ttl::Config::Device::getName() {
    if (this->name)
        return *this->name;

    return "";
}

void rfir::module::ttl::Config::Devices::free() {
    for (size_t i = 0; i < count; i++)
    {
        this->device[i].free();
    }    

    delete this->device;
    this->device = 0;
    this->count = 0;
}

bool  rfir::module::ttl::Config::Devices::parseFromJson(neb::CJsonObject* jp) {
    std::string key;
    int count = 0;
    while (!jp->IsEmpty() && jp->GetKey(key)) count++;
    this->free();
    this->device = new Device[count];
    this->count = count;
    int idx = 0;
    while (!jp->IsEmpty() && jp->GetKey(key))
    {
        neb::CJsonObject jdevice;
        if (jp->Get(key, jdevice)) 
            (this->device + idx)->parseFromJson(&jdevice, key.c_str());

        idx++;
    }
    return true;
}


rfir::module::ttl::Config::Device* rfir::module::ttl::Config::Devices::getDevice(std::string name) {
    if (this->count > 0) {        
        if (name == "")
            return this->device + 0;

        for (size_t i = 0; i < count; i++)
        {            
            if ((this->device + i)->getName() == name)
                return this->device + i;
        } 


    }

    return 0;
}

bool rfir::module::ttl::Config::Devices::clone(Devices* ds) {
    if (ds) {
        this->device = new Device[ds->count];
        this->count = ds->count;
        for (size_t i = 0; i < this->count; i++)
        {
            (this->device + i)->clone(ds->device + i);
        }
    }
    return false;
}
