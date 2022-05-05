#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
};

void GlobalConfig::fixup(){
    this->events.onFixup.emit((void*)this);
};


std::string GlobalConfig::getSvcTopic(std::string func) {
    return Config.app_id + "/" + Config.dom_id + "/+/+/dev/" + Config.dev_id + "/svc/" + func;   

};

void  GlobalConfig::getIds(neb::CJsonObject* pld){
    pld->ReplaceAdd("app_id", app_id);
    pld->ReplaceAdd("dom_id", dom_id);
    pld->ReplaceAdd("dsp_id", dsp_id);
    pld->ReplaceAdd("edg_id", edg_id);
};

GlobalConfig Config;