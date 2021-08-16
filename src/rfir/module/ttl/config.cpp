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
    setName(name);
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


void  rfir::module::ttl::Config::Device::setName(const char* value) {
    if (!this->name)
        this->name = new std::string();
    *this->name = value;
}

void rfir::module::ttl::Config::Devices::free() {
    for (size_t i = 0; i < getCount(); i++)
    {
        auto d = this->device[i];
        d->free();
        delete d;
    }    
    
    this->device.clear();
}

bool  rfir::module::ttl::Config::Devices::parseFromJson(neb::CJsonObject* jp) {
    std::string key;
    this->free();
    while (!jp->IsEmpty() && jp->GetKey(key))
    {
        neb::CJsonObject jdevice;
        if (jp->Get(key, jdevice)) {
            Device* d = new Device();
            d->parseFromJson(&jdevice, key.c_str());
            this->device.push_back(d);
        }
    }
    return true;
}


rfir::module::ttl::Config::Device* rfir::module::ttl::Config::Devices::getDevice(std::string name) {    
    if (this->getCount() > 0) {        
        for (size_t i = 0; i < getCount(); i++)
        {        
            if (this->device[i]->getName() == name) {
                return this->device[i];
            }
        }
        if (name == "")
            return this->device.front();        
    }

    return 0;
}

rfir::module::ttl::Config::Device* rfir::module::ttl::Config::Devices::newDevice(std::string name) {
    auto d = getDevice(name);
    if (d) {
        Serial.println(("创建设备失败，已存在：" + name).c_str());
        return 0;
    }
    d = new Device();
    d->setName(name.c_str());
    this->device.push_back(d);  
    return getDevice(name);

}

bool rfir::module::ttl::Config::Devices::clone(Devices* ds) {
    if (ds) {
        free();
        for (size_t i = 0; i < ds->getCount(); i++)
        {
            auto d = new Device();
            d->clone(ds->device[i]);
            this->device.push_back(d);
        }
        return true;
    }
    return false;
}


int rfir::module::ttl::Config::Devices::getCount() {
    return this->device.size();
}