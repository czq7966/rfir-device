#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
    // fixVars(mqtt_sub_topic);
    // fixVars(mqtt_pub_topic);
    // fixVars(mqtt_net_lgn_topic);
    // fixVars(mqtt_net_lgn_rsp_topic);
    // fixVars(mqtt_net_hbt_topic);
};

GlobalConfig::~GlobalConfig(){};

void GlobalConfig::fixVars(std::string& name) {
    String str = name.c_str(); str.replace("{did}", dev_id.c_str());
    name = str.c_str();    
};



GlobalConfig Config;