#include "config.h"
#include "rfir/util/util.h"
#include "rfir/util/file.h"

int GlobalConfig::Props::init(neb::CJsonObject* config){
    neb::CJsonObject temp;
    //id
    config->Get("app_id", this->app_id);
    config->Get("dom_id", this->dom_id);
    config->Get("dio_id", this->dio_id);
    config->Get("dsp_id", this->dsp_id);
    config->Get("edg_id", this->edg_id);
    config->Get("dev_id", this->dev_id);
    config->Get("dev_vendor", this->dev_vendor);
    config->Get("dev_model", this->dev_model);
    //pin
    config->Get("led_pin", this->led_pin);
    config->Get("reset_pin", this->reset_pin);
    config->Get("button_pin", this->button_pin);
    //serial
    config->Get("serial_baud", this->serial_baud);
    config->Get("serial_config", this->serial_config);
    config->Get("co_serial_baud", this->co_serial_baud);
    config->Get("co_serial_config", this->co_serial_config);
    //ota
    config->Get("ota_disable", this->ota_disable);
    config->Get("ota_update", this->ota_update);
    config->Get("ota_update_interval", this->ota_update_interval);
    config->Get("ota_update_url", this->ota_update_url);
    config->Get("ota_version_number", this->ota_version_number);
    config->Get("ota_version_string", this->ota_version_string);
    //wifi
    if (config->Get("wifi_ssid_dev", temp) && temp.IsArray()) {
        this->wifi_ssid_dev.clear();
        for(int i=0; i<temp.GetArraySize(); i++) {
            std::string str; temp.Get(i,str);
            if (str != "") this->wifi_ssid_dev.push_back(str);  
        }              
    }
    if (config->Get("wifi_password_dev", temp) && temp.IsArray()) {
        this->wifi_password_dev.clear();
        for(int i=0; i<temp.GetArraySize(); i++) {
            std::string str; temp.Get(i,str);
            if (str != "") this->wifi_password_dev.push_back(str); 
        }               
    }
    if (config->Get("wifi_ssid", temp) && temp.IsArray()) {
        this->wifi_ssid.clear();
        for(int i=0; i<temp.GetArraySize(); i++) {
            std::string str; temp.Get(i,str);
            if (str != "") this->wifi_ssid.push_back(str);    
        }            
    }
    if (config->Get("wifi_password", temp) && temp.IsArray()) {
        this->wifi_password.clear();
        for(int i=0; i<temp.GetArraySize(); i++){
            std::string str; temp.Get(i,str);
            if (str != "") this->wifi_password.push_back(str);     
        }           
    }

    //确保wifi ssid和pass数量一致
    if (this->wifi_ssid.size() != this->wifi_password.size()) {
        while(this->wifi_ssid.size() > this->wifi_password.size()) {
            this->wifi_password.push_back("");
        }
        while(this->wifi_ssid.size() < this->wifi_password.size()) {
            this->wifi_password.pop_back();
        }
    }

    config->Get("wifi_reset_timeout", this->wifi_reset_timeout);

    //ap
    config->Get("ap_config_pin", this->ap_config_pin);
    config->Get("ap_config_pin_timeout", this->ap_config_pin_timeout);
    config->Get("ap_config_version", this->ap_config_version);
    config->Get("ap_mode", this->ap_mode);
    config->Get("ap_password", this->ap_password);
    config->Get("ap_reset_timeout", this->ap_reset_timeout);
    config->Get("ap_ssid", this->ap_ssid);
    //smc
    config->Get("smc_config_file", this->smc_config_file);
    config->Get("smc_config_pin", this->smc_config_pin);
    config->Get("smc_config_pin_delay", this->smc_config_pin_delay);
    config->Get("smc_config_pin_interval", this->smc_config_pin_interval);
    config->Get("smc_config_pin_num", this->smc_config_pin_num);
    config->Get("smc_config_pin_type", this->smc_config_pin_type);
    config->Get("smc_config_timeout", this->smc_config_timeout);
    config->Get("smc_config_version", this->smc_config_version);
    config->Get("smc_disable", this->smc_disable);
    //mqtt
    config->Get("mqtt_buffer_size", this->mqtt_buffer_size);
    config->Get("mqtt_disable", this->mqtt_disable);
    config->Get("mqtt_ip", this->mqtt_ip);
    config->Get("mqtt_keepalive", this->mqtt_keepalive);
    config->Get("mqtt_clean_session", this->mqtt_clean_session);    
    config->Get("mqtt_password", this->mqtt_password);
    config->Get("mqtt_port", this->mqtt_port);
    config->Get("mqtt_reset_timeout", this->mqtt_reset_timeout);
    config->Get("mqtt_resp_timeout", this->mqtt_resp_timeout);
    config->Get("mqtt_user", this->mqtt_user);
    //mqtt topic
    config->Get("mqtt_dev_pub", this->mqtt_dev_pub);
    config->Get("mqtt_dev_status", this->mqtt_dev_status);
    config->Get("mqtt_dev_sub", this->mqtt_dev_sub);
    //time out
    config->Get("mqtt_dev_sub", this->device_timer_report_timeout);
    config->Get("mqtt_dev_sub", this->networking_re_handshake_timeout);
    config->Get("mqtt_dev_sub", this->networking_reset_timeout);

    return 1;
};

void GlobalConfig::Props::fixup(){
    fixupTopic();
};

void GlobalConfig::Props::fixupTopic() {
    //NETWORKING V3
    mqtt_dev_sub = expandTopic(MQTT_DEV_SUB);
    mqtt_dev_pub = expandTopic(MQTT_DEV_PUB);
    mqtt_dev_status = expandTopic(MQTT_DEV_STATUS);     

}

void GlobalConfig::Props::reset() {
    app_id = "";
    dom_id = "";
    dsp_id = "";
    edg_id = "";
    dio_id = "";
    dev_id = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);
};

std::string GlobalConfig::Props::expandTopic(std::string topic){
    String tpc = topic.c_str();
    tpc.replace("{app}", app_id == "" ? "0" : app_id.c_str());
    tpc.replace("{dom}", dom_id == "" ? "0" : dom_id.c_str());
    tpc.replace("{dsp}", dsp_id == "" ? "0" : dsp_id.c_str());
    tpc.replace("{edg}", edg_id == "" ? "0" : edg_id.c_str());
    tpc.replace("{dev}", dev_id == "" ? "0" : dev_id.c_str());
    tpc.replace("{dio}", dio_id == "" ? "0" : dio_id.c_str());
    return tpc.c_str();
};

GlobalConfig::GlobalConfig(){    
    reset();
    initFromFile();
    fixup();
};

int GlobalConfig::loadFromFile(neb::CJsonObject& config){
    rfir::util::TxtFile file(configFilename.c_str());
    std::string str;
    if (file.readString(str)) {
        return config.Parse(str);
    }
    return 0;
};

int GlobalConfig::saveToFile(neb::CJsonObject& config){
    rfir::util::TxtFile file(configFilename.c_str());
    std::string str = config.ToString();
    if (str.length() > 0) {
        return file.writeString(str);
    }
    return 0;
};

int GlobalConfig::initFromFile(){
    neb::CJsonObject config, app;
    this->loadFromFile(config);
    config.Get("app", app);
    return this->props.init(&app);
};


void GlobalConfig::fixup(){
    props.fixup();
    this->events.onFixup.emit((void*)this);
};

void GlobalConfig::fixupTopic() {
    props.fixupTopic();
}

void GlobalConfig::reset() {
    props.reset();
};

void  GlobalConfig::getIds(neb::CJsonObject* pld, std::string key){
    neb::CJsonObject ids;
    neb::CJsonObject* pids;
    if (key != "")  pids = &ids; else pids = pld;
    pids->ReplaceAdd("app", props.app_id);
    pids->ReplaceAdd("dom", props.dom_id);
    pids->ReplaceAdd("dsp", props.dsp_id);
    pids->ReplaceAdd("edg", props.edg_id);
    pids->ReplaceAdd("dev", props.dev_id);
    pids->ReplaceAdd("dio", props.dio_id);

    if (key != "") pld->ReplaceAdd(key, ids);

};

std::string GlobalConfig::expandTopic(std::string topic){
    return props.expandTopic(topic);
};

GlobalConfig Config;
JLed  GJLed = JLed(Config.props.led_pin);