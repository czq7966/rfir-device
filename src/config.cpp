#include "config.h"
#include "rfir/util/util.h"
#include "rfir/util/file.h"

int GlobalConfig::Props::init(JsonObject* _config){
    JsonObject config = *_config;
    if (config.isNull()) return 0;

    //id
    this->app_id = config.containsKey("app_id") ?  config["app_id"].as<std::string>() : this->app_id;
    this->dom_id = config.containsKey("dom_id") ?  config["dom_id"].as<std::string>() : this->dom_id;
    this->dio_id = config.containsKey("dio_id") ?  config["dio_id"].as<std::string>() : this->dio_id;
    this->dsp_id = config.containsKey("dsp_id") ?  config["dsp_id"].as<std::string>() : this->dsp_id;
    this->edg_id = config.containsKey("edg_id") ?  config["edg_id"].as<std::string>() : this->edg_id;
    this->dev_id = config.containsKey("dev_id") ?  config["dev_id"].as<std::string>() : this->dev_id;
    this->dev_vendor = config.containsKey("dev_vendor") ?  config["dev_vendor"].as<std::string>() : this->dev_vendor;
    this->dev_model = config.containsKey("dev_model") ?  config["dev_model"].as<std::string>() : this->dev_model;

    //pin
    this->led_pin = config.containsKey("led_pin") ?  config["led_pin"].as<int>() : this->led_pin;
    this->reset_pin = config.containsKey("reset_pin") ?  config["reset_pin"].as<int>() : this->reset_pin;
    this->button_pin = config.containsKey("button_pin") ?  config["button_pin"].as<int>() : this->button_pin;

    //serial
    this->serial_baud = config.containsKey("serial_baud") ?  config["serial_baud"].as<int>() : this->serial_baud;
    this->serial_config = config.containsKey("serial_config") ?  config["serial_config"].as<int>() : this->serial_config;
    this->co_serial_baud = config.containsKey("co_serial_baud") ?  config["co_serial_baud"].as<int>() : this->co_serial_baud;
    this->co_serial_config = config.containsKey("co_serial_config") ?  config["co_serial_config"].as<int>() : this->co_serial_config;

    //ota
    this->ota_disable = config.containsKey("ota_disable") ?  config["ota_disable"].as<bool>() : this->ota_disable;
    this->ota_update = config.containsKey("ota_update") ?  config["ota_update"].as<bool>() : this->ota_update;
    this->ota_update_interval = config.containsKey("ota_update_interval") ?  config["ota_update_interval"].as<int>() : this->ota_update_interval;
    this->ota_update_url = config.containsKey("ota_update_url") ?  config["ota_update_url"].as<std::string>() : this->ota_update_url;
    this->ota_version_number = config.containsKey("ota_version_number") ?  config["ota_version_number"].as<int>() : this->ota_version_number;
    this->ota_version_string = config.containsKey("ota_version_string") ?  config["ota_version_string"].as<std::string>() : this->ota_version_string;

    //wifi    
    if (config.containsKey("wifi_ssid_dev")) {
        JsonArray temp = config["wifi_ssid_dev"];
        if (!temp.isNull()) {
            this->wifi_ssid_dev.clear();
            for(int i=0; i< temp.size(); i++) {
                std::string str = temp[i];
                if (str != "") this->wifi_ssid_dev.push_back(str);  
            }    
        }          
    }
     
    if (config.containsKey("wifi_password_dev")) {
        JsonArray temp = config["wifi_password_dev"];
        if (!temp.isNull()) {
            this->wifi_password_dev.clear();
            for(int i=0; i<temp.size(); i++) {
                std::string str = temp[i];
                if (str != "") this->wifi_password_dev.push_back(str); 
            }         
        }      
    }

    if (config.containsKey("wifi_ssid")) {
        JsonArray temp = config["wifi_ssid"];
        if (!temp.isNull()) {
            this->wifi_ssid.clear();
            for(int i=0; i<temp.size(); i++) {
                std::string str = temp[i];
                if (str != "") this->wifi_ssid.push_back(str);    
            }   
        }         
    }

    if (config.containsKey("wifi_password")) {
        JsonArray temp = config["wifi_password"];
        if (!temp.isNull()) {
            this->wifi_password.clear();
            for(int i=0; i<temp.size(); i++){
                std::string str = temp[i];
                if (str != "") this->wifi_password.push_back(str);     
            }     
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


    this->wifi_reset_timeout = config.containsKey("wifi_reset_timeout") ?  config["wifi_reset_timeout"].as<int>() : this->wifi_reset_timeout;

    //ap
    this->ap_config_pin = config.containsKey("ap_config_pin") ?  config["ap_config_pin"].as<int>() : this->ap_config_pin;
    this->ap_config_pin_timeout = config.containsKey("ap_config_pin_timeout") ?  config["ap_config_pin_timeout"].as<int>() : this->ap_config_pin_timeout;
    this->ap_config_version = config.containsKey("ap_config_version") ?  config["ap_config_version"].as<std::string>() : this->ap_config_version;
    this->ap_mode = config.containsKey("ap_mode") ?  config["ap_mode"].as<bool>() : this->ap_mode;
    this->ap_password = config.containsKey("ap_password") ?  config["ap_password"].as<std::string>() : this->ap_password;
    this->ap_reset_timeout = config.containsKey("ap_reset_timeout") ?  config["ap_reset_timeout"].as<int>() : this->ap_reset_timeout;
    this->ap_ssid = config.containsKey("ap_ssid") ?  config["ap_ssid"].as<std::string>() : this->ap_ssid;


    //smc
    this->smc_config_file = config.containsKey("smc_config_file") ?  config["smc_config_file"].as<std::string>() : this->smc_config_file;
    this->smc_config_pin = config.containsKey("smc_config_pin") ?  config["smc_config_pin"].as<int>() : this->smc_config_pin;
    this->smc_config_pin_delay = config.containsKey("smc_config_pin_delay") ?  config["smc_config_pin_delay"].as<int>() : this->smc_config_pin_delay;
    this->smc_config_pin_interval = config.containsKey("smc_config_pin_interval") ?  config["smc_config_pin_interval"].as<int>() : this->smc_config_pin_interval;
    this->smc_config_pin_num = config.containsKey("smc_config_pin_num") ?  config["smc_config_pin_num"].as<int>() : this->smc_config_pin_num;
    this->smc_config_pin_type = config.containsKey("smc_config_pin_type") ?  config["smc_config_pin_type"].as<int>() : this->smc_config_pin_type;
    this->smc_config_timeout = config.containsKey("smc_config_timeout") ?  config["smc_config_timeout"].as<int>() : this->smc_config_timeout;
    this->smc_config_version = config.containsKey("smc_config_version") ?  config["smc_config_version"].as<std::string>() : this->smc_config_version;
    this->smc_disable = config.containsKey("smc_disable") ?  config["smc_disable"].as<bool>() : this->smc_disable;

    //mqtt
    this->mqtt_buffer_size = config.containsKey("mqtt_buffer_size") ?  config["mqtt_buffer_size"].as<int>() : this->mqtt_buffer_size;
    this->mqtt_disable = config.containsKey("mqtt_disable") ?  config["mqtt_disable"].as<bool>() : this->mqtt_disable;
    this->mqtt_ip = config.containsKey("mqtt_ip") ?  config["mqtt_ip"].as<std::string>() : this->mqtt_ip;
    this->mqtt_keepalive = config.containsKey("mqtt_keepalive") ?  config["mqtt_keepalive"].as<int>() : this->mqtt_keepalive;
    this->mqtt_clean_session = config.containsKey("mqtt_clean_session") ?  config["mqtt_clean_session"].as<int>() : this->mqtt_clean_session;
    this->mqtt_password = config.containsKey("mqtt_password") ?  config["mqtt_password"].as<std::string>() : this->mqtt_password;
    this->mqtt_port = config.containsKey("mqtt_port") ?  config["mqtt_port"].as<int>() : this->mqtt_port;
    this->mqtt_reset_timeout = config.containsKey("mqtt_reset_timeout") ?  config["mqtt_reset_timeout"].as<int>() : this->mqtt_reset_timeout;
    this->mqtt_resp_timeout = config.containsKey("mqtt_resp_timeout") ?  config["mqtt_resp_timeout"].as<int>() : this->mqtt_resp_timeout;
    this->mqtt_user = config.containsKey("mqtt_user") ?  config["mqtt_user"].as<std::string>() : this->mqtt_user;

    //mqtt topic
    this->mqtt_dev_pub = config.containsKey("mqtt_dev_pub") ?  config["mqtt_dev_pub"].as<std::string>() : this->mqtt_dev_pub;
    this->mqtt_dev_status = config.containsKey("mqtt_dev_status") ?  config["mqtt_dev_status"].as<std::string>() : this->mqtt_dev_status;
    this->mqtt_dev_sub = config.containsKey("mqtt_dev_sub") ?  config["mqtt_dev_sub"].as<std::string>() : this->mqtt_dev_sub;

    //time out
    this->device_timer_report_timeout = config.containsKey("device_timer_report_timeout") ?  config["device_timer_report_timeout"].as<int>() : this->device_timer_report_timeout;
    this->networking_re_handshake_timeout = config.containsKey("networking_re_handshake_timeout") ?  config["networking_re_handshake_timeout"].as<int>() : this->networking_re_handshake_timeout;
    this->networking_reset_timeout = config.containsKey("networking_reset_timeout") ?  config["networking_reset_timeout"].as<int>() : this->networking_reset_timeout;

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

int GlobalConfig::loadFromFile(JsonObject& config){
    rfir::util::TxtFile file(configFilename.c_str());
    std::string str;
    if (file.readString(str)) {
        DynamicJsonDocument doc(MQTT_BUFFER_SIZE);
        deserializeJson(doc, str);
        return config.set(doc.as<JsonObject>());
    }
    return 0;
};

int GlobalConfig::saveToFile(JsonObject& config){
    rfir::util::TxtFile file(configFilename.c_str());
    std::string str;
    serializeJson(config, str);
    if (str.length() > 0) {
        return file.writeString(str);
    }
    return 0;
};

int GlobalConfig::initFromFile(){
    DynamicJsonDocument doc(Config.props.mqtt_buffer_size);
    JsonObject config = doc.to<JsonObject>();    
    this->loadFromFile(config);
    JsonObject app = config["app"];
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


void  GlobalConfig::getIds(JsonObject* _pld, std::string key){
    if (!_pld) return;

    JsonObject pld = *_pld;
    JsonObject ids = pld;

    if (key != "") 
        ids = pld.containsKey(key) ? pld[key] : pld.createNestedObject(key);
    

    ids["app"] = props.app_id;
    ids["dom"] = props.dom_id;
    ids["dsp"] = props.dsp_id;
    ids["edg"] = props.edg_id;
    ids["dev"] = props.dev_id;
    ids["dio"] = props.dio_id;

};


std::string GlobalConfig::expandTopic(std::string topic){
    return props.expandTopic(topic);
};

GlobalConfig Config;
JLed  GJLed = JLed(Config.props.led_pin);