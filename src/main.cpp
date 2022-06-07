#include <Arduino.h>
#include "config.h"
#include "global.h"
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

#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/cmds/cmd.h"

#include "rfir/util/event-timer.h"
#include "rfir/util/led.h"

#include "cmds/cmd/cmd-base.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "cmds/network/mqtt-signaler.h"


#include "service/cmds/cmd.h"


std::string ChipID = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);

void* OnConfigFixup(void* arg, void* p) {
    DEBUGER.println("OnConfigFixup");
    cmds::cmd::CmdBase::Command::DefaultFrom->type ="dev";
    cmds::cmd::CmdBase::Command::DefaultFrom->id = Config.dev_id;
    if (Config.edg_id != "") {
        cmds::cmd::CmdBase::Command::DefaultTo->type ="edg";
        cmds::cmd::CmdBase::Command::DefaultTo->id = Config.edg_id;
    }

    cmds::cmd::CmdBase::Command::DefaultRespTimeout = Config.mqtt_resp_timeout;
    cmds::cmd::CmdMqtt::topicPrefix = cmds::cmd::CmdMqtt::ZeroTopic(Config.app_id) + "/" + cmds::cmd::CmdMqtt::ZeroTopic(Config.dom_id) + "/";

    return 0;
}

void setup() {
#ifdef SERIAL_BAUD
    DEBUGER.begin(SERIAL_BAUD, SERIAL_CONFIG);
#else 
   DEBUGER.begin(115200);
#endif
    DEBUGER.println("begin chid id: " + String(ChipID.c_str()) + " , mac: " + String(rfir::util::Util::GetMacAddress().c_str()));
    Global::Init();

#if !(defined(DISABLE_WIFI) && DISABLE_WIFI == TRUE)
    //启动wifi或热点
    network::module::wifi::Client::Params np;    
    np.ssid = Config.wifi_ssid;
    np.pass = Config.wifi_password;
    np.timeout = Config.wifi_reset_timeout;
    //AP
    #ifdef AP_MODE
    np.apMode = Config.ap_mode;
    np.ap.apSsid = Config.ap_ssid == "" ? ChipID : Config.ap_ssid;
    np.ap.apPass = Config.ap_password;
    np.ap.resetTimeout = Config.ap_reset_timeout;
    np.ap.configVersion = Config.ap_config_version;
    np.ap.configPin = Config.ap_config_pin;
    np.ap.configPinTimeout = Config.ap_config_pin_timeout;
    #endif

    for(auto it = Config.wifi_ssid_dev.begin(); it != Config.wifi_ssid_dev.end(); it++) {
        np.ssid.push_back(*it);
    }

    for(auto it = Config.wifi_password_dev.begin(); it != Config.wifi_password_dev.end(); it++) {
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

    

#endif

#if !(defined(DISABLE_OTA) && DISABLE_OTA == TRUE)
    //启动OTA
    network::module::ota::Updater::Params op;
    op.url = OTA_UPDATE_URL;
    op.id = ChipID;
    op.version = OTA_VERSION_NUMBER;
    op.interval = OTA_UPDATE_INTERVAL;
    GOTAUpdater.start(op);
#endif

#if !(defined(DISABLE_MQTT) && DISABLE_MQTT == TRUE)
    //启动mqtt
    network::module::mqtt::AClient::Params mp;
    mp.ip = MQTT_IP;
    mp.port = MQTT_PORT;
    mp.timeout = MQTT_RESET_TIMEOUT;
    mp.user = MQTT_USER;
    mp.pass = MQTT_PASSWORD;
    mp.id = ChipID;
    #ifdef MQTT_KEEPALIVE
    mp.keepalive = MQTT_KEEPALIVE;
    #endif

    GCmdDispatcher.setSignaler(&GMqttSignaler);
    GMqttSignaler.setMqtt(&GMqttClient);
    GMqttClient.start(mp);
    // GCmdDispatcher.events.onConnect.add((void*)&onMqttConnect, onMqttConnect);

    // network::service::mqtt::AClient::Start(mp);
#endif
    //启动组网组件
    GNetworking.start();

    //启动收发器
    // rfir::Start(onRfirStart, onRfirSniffed, onRfirDecoded, onRfirEncoded, onRfirSended);


    //启动设备
    GDevice->start(0);

    // //启动 设备
    // rfir::service::device::Device::Start(onDeviceChange);
    

    //业务开始
    service::cmds::Cmd::Start();

    //定时器
    GEventTimer.start();

    //全局配置, 得先于其他组件
    Config.events.onFixup.add(0, OnConfigFixup, 0);    
    Config.fixup();
}



void loop() {
// #if !(defined(DISABLE_WIFI) && DISABLE_WIFI == TRUE)    
//     //wifi循环
//     // network::service::wifi::Client::Loop();
//     GWifiClient.loop();
//     #if !(defined(DISABLE_SMC) && DISABLE_SMC == TRUE)
//     // GSmartConfig.loop();
//     #endif
// #endif


// #if !(defined(DISABLE_MQTT) && DISABLE_MQTT == TRUE)
//     //mqtt循环
//     // network::service::mqtt::AClient::Loop();
//     GMqttClient.loop();
// #endif    

    // //组网组件循环
    // GNetworking.loop();

    // //收发器循环
    // rfir::Loop();  

    //设备循环
    GDevice->loop();
    //业务循环
    // service::cmds::Cmd::Loop();

    //定时器
    GEventTimer.loop();
    //LED
    GLed.loop();
}


