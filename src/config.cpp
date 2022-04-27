#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
};

void GlobalConfig::fixup(){
    this->events.onFixup.emit((void*)this);
};

GlobalConfig::~GlobalConfig(){};

void GlobalConfig::fixVars(std::string& name) {
    // String str = name.c_str(); 
    // str.replace("{domain}", mqtt_domain.c_str()); 
    // str.replace("{did}", dev_id.c_str());
    
    // name = str.c_str();  
};


GlobalConfig Config;