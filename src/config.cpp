#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){    
    reset();
    fixup();
};

void GlobalConfig::fixup(){
    fixupTopic();
    this->events.onFixup.emit((void*)this);
};

void GlobalConfig::fixupTopic() {
    mqtt_dsp_evt_status = expandTopic(MQTT_DSP_EVT_STATUS);
    mqtt_edg_evt_status = expandTopic(MQTT_EDG_EVT_STATUS);

    //DEV.sub
    mqtt_dev_svc_login = expandTopic(MQTT_DEV_SVC_LOGIN);
    mqtt_dev_svc_handshake = expandTopic(MQTT_DEV_SVC_HANDSHAKE);
    mqtt_dev_svc_penet = expandTopic(MQTT_DEV_SVC_PENET);
    mqtt_dev_svc_get = expandTopic(MQTT_DEV_SVC_GET);
    mqtt_dev_svc_set = expandTopic(MQTT_DEV_SVC_SET);
    mqtt_dev_svc_reboot = expandTopic(MQTT_DEV_SVC_REBOOT);    

}

void GlobalConfig::reset() {
    app_id = "";
    dom_id = "";
    dsp_id = "";
    edg_id = "";
    dio_id = "";
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
};

// std::string GlobalConfig::getSvcTopic(std::string func) {
//     return Config.app_id + "/" + Config.dom_id + "/+/+/dev/" + Config.dev_id + "/svc/" + func;   

// };


// std::string GlobalConfig::getEdgStatusTopic() {
//     if (Config.edg_id != "") {
//         return Config.app_id + "/" + Config.dom_id + "/edg/" + Config.edg_id + "/0/0/evt/" + mqtt_edg_evt_status;
//     }
//     return "";
// };



void  GlobalConfig::getIds(neb::CJsonObject* pld, std::string key){
    neb::CJsonObject ids;
    neb::CJsonObject* pids;
    if (key != "")  pids = &ids; else pids = pld;
    pids->ReplaceAdd("app", app_id);
    pids->ReplaceAdd("dom", dom_id);
    pids->ReplaceAdd("dsp", dsp_id);
    pids->ReplaceAdd("edg", edg_id);
    pids->ReplaceAdd("dev", dev_id);
    pids->ReplaceAdd("dio", dio_id);

    if (key != "") pld->ReplaceAdd(key, ids);

};

std::string GlobalConfig::expandTopic(std::string topic){
    String tpc = topic.c_str();
    tpc.replace("{app}", app_id == "" ? "0" : app_id.c_str());
    tpc.replace("{dom}", dom_id == "" ? "0" : dom_id.c_str());
    tpc.replace("{dsp}", dsp_id == "" ? "0" : dsp_id.c_str());
    tpc.replace("{edg}", edg_id == "" ? "0" : edg_id.c_str());
    tpc.replace("{dev}", dev_id == "" ? "0" : dev_id.c_str());
    tpc.replace("{dio}", dio_id == "" ? "0" : dio_id.c_str());
    return tpc.c_str();
};

GlobalConfig Config;