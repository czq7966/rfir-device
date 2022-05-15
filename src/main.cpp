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

#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/cmds/cmd.h"

#include "rfir/util/event-timer.h"
#include "rfir/rfir.h"

#include "cmds/cmd/cmd-base.h"
#include "cmds/cmd/cmd-dispatcher.h"
#include "cmds/network/mqtt-signaler.h"


#include "service/cmds/cmd.h"


std::string ChipID = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);

void onRfirSniffed(rfir::module::ttl::Sniffer* sniffer, rfir::module::ttl::Delta* data, int count) {
    // DEBUGER.println("onRfirSniffed");
    const char * payload;
#ifdef DEBUG_RFIR
    if (!payload) payload = rfir::module::ttl::Sniffer::packSniffedCmd(sniffer, sniffer->toString().c_str()).c_str();
    DEBUGER.println(payload);
#endif    

    // if (sniffer->getSniffParams()->response) {
    //     if (!payload) payload = rfir::module::ttl::Sniffer::packSniffedCmd(sniffer, sniffer->toString().c_str()).c_str();
    //     network::service::mqtt::Client::Publish(payload);
    // } 
    
}

void onRfirDecoded(rfir::module::ttl::Decoder* decoder, rfir::module::ttl::Decoder::DecodeResults* data) {
    DEBUGER.println("onRfirDecoded");
    const char * payload;
#ifdef DEBUG_RFIR    
    if (!payload) payload = rfir::module::ttl::Decoder::packDecodedCmd(decoder, data).c_str();
    DEBUGER.println(payload);
    // DEBUGER.println(data->toJsonString().c_str());
    // DEBUGER.println(data->toJsonString(true).c_str());
#endif    

    // if (decoder->getDecodeParams()->response) {
    //     if (!payload) payload = rfir::module::ttl::Decoder::packDecodedCmd(decoder, data).c_str();
    //     network::service::mqtt::Client::Publish(payload);
    // } 

    // if (data->count > 0) 
    //     service::cmds::Cmd::OnCmd_decoded(data);
    return;
}

void onRfirEncoded(rfir::module::ttl::Encoder* encoder, rfir::module::ttl::Encoder::EncodeResult* data) {
    DEBUGER.println("onRfirEncoded: " + String(data->count));
    const char * payload;
#ifdef DEBUG_RFIR    
    if (!payload) payload = rfir::module::ttl::Encoder::packEncodedCmd(encoder, data).c_str();
    DEBUGER.println(payload);
#endif

    // if (encoder->getEncodeParams()->response) {
    //     if (!payload) payload = rfir::module::ttl::Encoder::packEncodedCmd(encoder, data).c_str();
    //     network::service::mqtt::Client::Publish(payload);
    // } 

}

void onRfirSended(rfir::module::ttl::Sender* sender, const uint16_t* data, const uint16_t len) {
    DEBUGER.println("onRfirSended: " + String(len));

    // if (sender->getSendParams()->response) {
    //     network::service::mqtt::Client::Publish(rfir::module::ttl::Sender::packSendedCmd(sender, data, len).c_str());
    // }
    
}

// void* onDeviceChange(void* arg, void* p) {
//     auto reason = (const char*)p;
//     DEBUGER.printf("onDeviceChange: %s\n", reason);
//     service::cmds::Cmd::OnCmd_get((::cmds::cmd::CmdMqtt*)0, reason);
//     return 0;
// }

void onRfirStart(void* data) {
    DEBUGER.println("onRfirStart");

    //初始化设备
    auto d = GDevice->init();
    rfir::service::cmds::Cmd::onCmd_sniff(d);
    rfir::service::cmds::Cmd::onCmd_send(d);
}

// std::string getMqttSvcTopic(std::string func) {
//     return Config.app_id + "/" + Config.dom_id + "/+/+/dev/" + Config.dev_id + "/svc/" + func;

// }



// uint16_t onMqttConnect_count = 0;
// void onMqttConnect(network::module::mqtt::Client::MQTT* mqtt) {
//     onMqttConnect_count++;
//     DEBUGER.println("onMqttConnect");
//     if (onMqttConnect_count > 1)
//         service::cmds::Cmd::OnCmd_heartbeat(0, 1);
//     else 
//         service::cmds::Cmd::OnCmd_heartbeat(0, 2);
//     service::cmds::Cmd::DoTimerReport(true);

//     doMqttSubscribe(mqtt);

// }

// void* onMqttConnect(void* arg, void* p) {
//     doMqttSubscribe(GMqttSignaler.mqtt);
//     return 0;
// }



// void onMqttMessage(MQTTClient *client, char topic[], char bytes[], int length) {
//     service::cmds::Cmd::OnCmd(bytes);
// }

void* OnConfigFixup(void* arg, void* p) {
    DEBUGER.println("OnConfigFixup");
    cmds::cmd::CmdBase::Command::DefaultFrom->type ="dev";
    cmds::cmd::CmdBase::Command::DefaultFrom->id = Config.dev_id;
    if (Config.edg_id != "") {
        cmds::cmd::CmdBase::Command::DefaultTo->type ="edg";
        cmds::cmd::CmdBase::Command::DefaultTo->id = Config.edg_id;
    }

    cmds::cmd::CmdBase::Command::DefaultRespTimeout = Config.mqtt_resp_timeout;
    cmds::cmd::CmdMqtt::topicPrefix = Config.app_id + "/" + Config.dom_id + "/";

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
    np.ssid = WIFI_SSID;                               
    np.pass = WIFI_PASSWORD;            
    np.timeout = WIFI_RESET_TIMEOUT;    
    //AP
    #ifdef AP_MODE
    np.apMode = AP_MODE; 
    np.ap.apSsid = AP_SSID == "" ? ChipID : AP_SSID;    
    np.ap.apPass = AP_PASSWORD;         
    np.ap.resetTimeout = AP_RESET_TIMEOUT;
    np.ap.configVersion = AP_CONFIG_VERSION;            
    np.ap.configPin = AP_CONFIG_PIN;    
    np.ap.configPinTimeout = AP_CONFIG_PIN_TIMEOUT;
    #endif

    // network::service::wifi::Client::Start(np);

    #ifdef WIFI_SSID_DEV
    np.ssid.push_back(WIFI_SSID_DEV);
    np.pass.push_back(WIFI_PASSWORD_DEV);    
    #endif

    GWifiClient.start(np);

    //SMC
    #if !(defined(DISABLE_SMC) && DISABLE_SMC == TRUE)
    auto smc = &GWifiClient.params.smc;
    smc->configVersion = SMC_CONFIG_VERSION;
    smc->configFile = SMC_CONFIG_FILE; 
    smc->configPin = SMC_CONFIG_PIN;  
    smc->configPinType = SMC_CONFIG_PIN_TYPE;
    smc->configPinNum = SMC_CONFIG_PIN_NUM;
    smc->configPinDelay = SMC_CONFIG_PIN_DELAY;
    smc->configPinInterval = SMC_CONFIG_PIN_INTERVAL;
    smc->configTimeout = SMC_CONFIG_TIMEOUT;
    // GSmartConfig.start(smc);
    #endif   

    

#endif

#if !(defined(DISABLE_OTA) && DISABLE_OTA == TRUE)
    //启动OTA
    network::module::ota::Updater::Params op;
    op.url = OTA_UPDATE_URL;
    op.id = ChipID;
    op.version = OTA_VERSION_NUMBER;
    network::service::ota::Updater::Start(op);
#endif

#if !(defined(DISABLE_MQTT) && DISABLE_MQTT == TRUE)
    //启动mqtt
    network::module::mqtt::AClient::Params mp;
    mp.ip = MQTT_IP;
    mp.port = MQTT_PORT;
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
#if !(defined(DISABLE_WIFI) && DISABLE_WIFI == TRUE)    
    //wifi循环
    // network::service::wifi::Client::Loop();
    GWifiClient.loop();
    #if !(defined(DISABLE_SMC) && DISABLE_SMC == TRUE)
    // GSmartConfig.loop();
    #endif
#endif

#if !(defined(DISABLE_OTA) && DISABLE_OTA == TRUE)
    //OTA循环
    network::service::ota::Updater::Loop();
#endif

#if !(defined(DISABLE_MQTT) && DISABLE_MQTT == TRUE)
    //mqtt循环
    // network::service::mqtt::AClient::Loop();
    GMqttClient.loop();
#endif    

    //组网组件循环
    GNetworking.loop();

    // //收发器循环
    // rfir::Loop();  

    //设备循环
    GDevice->loop();
    //业务循环
    // service::cmds::Cmd::Loop();

    //定时器
    GEventTimer.loop();
}


