#include <Arduino.h>
#include "config.h"
#include <IotWebConf.h>
#include "network/module/wifi/client.h"
#include "network/module/ota/updater.h"
#include "network/service/wifi/client.h"
#include "network/service/mqtt/async-client.h"
#include "network/service/ota/updater.h"

#include "rfir/module/device/networking.h"
#include "rfir/module/device/device.h"
#include "rfir/module/device/ac/midea/fz-dm-midea.h"
#include "rfir/module/ttl/sender-v2.h"

#include "rfir/util/event-timer.h"
#include "rfir/util/led.h"

#include "cmds/cmd/cmd-base.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "cmds/network/mqtt-signaler.h"


#include "service/cmds/cmd.h"


void* OnConfigFixup(void* arg, void* p) {
    DEBUGER.println("OnConfigFixup");
    cmds::cmd::CmdBase::Command::DefaultFrom->type ="dev";
    cmds::cmd::CmdBase::Command::DefaultFrom->id = Config.props.dev_id;
    if (Config.props.edg_id != "") {
        cmds::cmd::CmdBase::Command::DefaultTo->type ="edg";
        cmds::cmd::CmdBase::Command::DefaultTo->id = Config.props.edg_id;
    }

    cmds::cmd::CmdBase::Command::DefaultRespTimeout = Config.props.mqtt_resp_timeout;
    cmds::cmd::CmdMqtt::topicPrefix = cmds::cmd::CmdMqtt::ZeroTopic(Config.props.app_id) + "/" + cmds::cmd::CmdMqtt::ZeroTopic(Config.props.dom_id) + "/";

    return 0;
}

void setup() {
    Serial.begin(Config.props.serial_baud, SerialConfig(Config.props.serial_config));
    DEBUGER.println(String(Config.props.serial_baud)+ ":" + String(Config.props.serial_config));
    DEBUGER.println("begin chid id: " + String(Config.props.dev_id.c_str()) + " , mac: " + String(rfir::util::Util::GetMacAddress().c_str()));

    if (!Config.props.wifi_disable) {
        //启动wifi或热点
        network::module::wifi::Client::Params np;    
        np.ssid = Config.props.wifi_ssid;
        np.pass = Config.props.wifi_password;
        np.timeout = Config.props.wifi_reset_timeout;
        //AP
        if (Config.props.ap_mode) {
            np.apMode = Config.props.ap_mode;
            np.ap.apSsid = Config.props.ap_ssid == "" ? Config.props.dev_id : Config.props.ap_ssid;
            np.ap.apPass = Config.props.ap_password;
            np.ap.resetTimeout = Config.props.ap_reset_timeout;
            np.ap.configVersion = Config.props.ap_config_version;
            np.ap.configPin = Config.props.ap_config_pin;
            np.ap.configPinTimeout = Config.props.ap_config_pin_timeout;
        }

        for(auto it = Config.props.wifi_ssid_dev.begin(); it != Config.props.wifi_ssid_dev.end(); it++) {
            np.ssid.push_back(*it);
        }

        for(auto it = Config.props.wifi_password_dev.begin(); it != Config.props.wifi_password_dev.end(); it++) {
            np.pass.push_back(*it);
        }

        GWifiClient.start(np);

        // //SMC
        // #if !(defined(DISABLE_SMC) && DISABLE_SMC == TRUE)
        // auto smc = &GWifiClient.params.smc;
        // smc->configVersion = SMC_CONFIG_VERSION;
        // smc->configFile = SMC_CONFIG_FILE; 
        // smc->configPin = SMC_CONFIG_PIN;  
        // smc->configPinType = SMC_CONFIG_PIN_TYPE;
        // smc->configPinNum = SMC_CONFIG_PIN_NUM;
        // smc->configPinDelay = SMC_CONFIG_PIN_DELAY;
        // smc->configPinInterval = SMC_CONFIG_PIN_INTERVAL;
        // smc->configTimeout = SMC_CONFIG_TIMEOUT;
        // // GSmartConfig.start(smc);
        // #endif      

    }

    if (!Config.props.ota_disable) {
        //启动OTA
        network::module::ota::Updater::Params op;
        op.url = Config.props.ota_update_url;
        op.id = Config.props.dev_id;
        op.version = Config.props.ota_version_number;
        op.interval = Config.props.ota_update_interval;
        GOTAUpdater.start(op);
    }

    if (!Config.props.mqtt_disable) {
        //启动mqtt
        network::module::mqtt::AClient::Params mp;
        mp.ip = Config.props.mqtt_ip; 
        mp.port = Config.props.mqtt_port; 
        mp.timeout = Config.props.mqtt_reset_timeout; 
        mp.user = Config.props.mqtt_user; 
        mp.pass = Config.props.mqtt_password; 
        mp.id = Config.props.dev_id; 
        mp.keepalive = Config.props.mqtt_keepalive;
        mp.clean_session = Config.props.mqtt_clean_session;

        GCmdDispatcher.setSignaler(&GMqttSignaler);
        GMqttSignaler.setMqtt(&GMqttClient);
        GMqttClient.start(mp);
    }
    //启动组网组件
    GNetworking.start();


    //启动设备
    GDevice->start(0);


    //业务开始
    service::cmds::Cmd::Start();

    //定时器
    GEventTimer.start();

    //全局配置, 得先于其他组件
    Config.events.onFixup.add(0, OnConfigFixup, 0);    
    Config.fixup();
}



void loop() {
    //设备循环
    GDevice->loop();
    //业务循环
    //定时器
    GEventTimer.loop();
    //LED
    GLed.loop();
}


