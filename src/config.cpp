#include "config.h"
#include "rfir/util/util.h"

GlobalConfig::GlobalConfig(){
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
    fixup();
};

void GlobalConfig::fixup(){
    fixupTopic();
    this->events.onFixup.emit((void*)this);
};

void GlobalConfig::fixupTopic() {
    //DSP.pub
    // mqtt_dsp_svc_login = expandTopic(MQTT_DSP_SVC_LOGIN);
    //DSP.sub
    mqtt_dsp_evt_status = expandTopic(MQTT_DSP_EVT_STATUS);

    //EDG.pub
    // mqtt_edg_svc_handshake = expandTopic(MQTT_EDG_SVC_HANDSHAKE);
    //EDG.sub
    mqtt_edg_evt_status = expandTopic(MQTT_EDG_EVT_STATUS);


    //DEV.pub
    // mqtt_dev_evt_report = expandTopic(MQTT_DEV_EVT_REPORT);
    // mqtt_dev_evt_status = expandTopic(MQTT_DEV_EVT_STATUS);
    // mqtt_dev_evt_penet = expandTopic(MQTT_DEV_EVT_PENET);

    //DEV.sub
    mqtt_dev_svc_login = expandTopic(MQTT_DEV_SVC_LOGIN);
    mqtt_dev_svc_handshake = expandTopic(MQTT_DEV_SVC_HANDSHAKE);
    mqtt_dev_svc_get = expandTopic(MQTT_DEV_SVC_GET);
    mqtt_dev_svc_set = expandTopic(MQTT_DEV_SVC_SET);
    mqtt_dev_svc_reboot = expandTopic(MQTT_DEV_SVC_REBOOT);
    mqtt_dev_svc_penet = expandTopic(MQTT_DEV_SVC_PENET);

}


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
    pids->ReplaceAdd("app_id", app_id);
    pids->ReplaceAdd("dom_id", dom_id);
    pids->ReplaceAdd("dsp_id", dsp_id);
    pids->ReplaceAdd("edg_id", edg_id);
    pids->ReplaceAdd("dev_id", dev_id);

    if (key != "") pld->ReplaceAdd(key, ids);

};

std::string GlobalConfig::expandTopic(std::string topic){
    String tpc = topic.c_str();
    tpc.replace("{app}", app_id == "" ? "0" : app_id.c_str());
    tpc.replace("{dom}", dom_id == "" ? "0" : dom_id.c_str());
    tpc.replace("{dsp}", dsp_id == "" ? "0" : dsp_id.c_str());
    tpc.replace("{edg}", edg_id == "" ? "0" : edg_id.c_str());
    tpc.replace("{dev}", dev_id == "" ? "0" : dev_id.c_str());
    return tpc.c_str();
};

GlobalConfig Config;