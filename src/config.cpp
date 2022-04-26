#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);

    fixVars(mqtt_dsp_svc_login);
    fixVars(mqtt_edg_svc_handshake);

    fixVars(mqtt_dev_svc_login);
    fixVars(mqtt_dev_svc_handshake);
    fixVars(mqtt_dev_svc_get);
    fixVars(mqtt_dev_svc_set);

    fixVars(mqtt_dev_evt_heartbeat);
    fixVars(mqtt_dev_evt_reboot);
    fixVars(mqtt_dev_evt_reconnect);
    fixVars(mqtt_dev_evt_report);
};

GlobalConfig::~GlobalConfig(){};

void GlobalConfig::fixVars(std::string& name) {
    String str = name.c_str(); 
    str.replace("{domain}", mqtt_domain.c_str()); 
    str.replace("{did}", dev_id.c_str());
    
    name = str.c_str();  
};


GlobalConfig Config;