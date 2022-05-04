#include <Arduino.h>
#include "config.h"
#include "global.h"
#include <IotWebConf.h>
#include "network/module/wifi/client.h"
#include "network/module/ota/updater.h"
#include "network/service/wifi/client.h"
#include "network/service/mqtt/async-client.h"
#include "network/service/ota/updater.h"

#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/cmds/cmd.h"
#include "rfir/service/device/device.h"
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

    if (data->count > 0) 
        service::cmds::Cmd::OnCmd_decoded(data);
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

void* onDeviceChange(void* arg, void* p) {
    auto reason = (const char*)p;
    DEBUGER.printf("onDeviceChange: %s\n", reason);
    service::cmds::Cmd::OnCmd_get((::cmds::cmd::CmdMqtt*)0, reason);
    return 0;
}

void onRfirStart(void* data) {
    DEBUGER.println("onRfirStart");

    //初始化设备
    auto d = rfir::service::device::Device::Init();
    rfir::service::cmds::Cmd::onCmd_sniff(d);
    rfir::service::cmds::Cmd::onCmd_send(d);
}

std::string getMqttSvcTopic(std::string func) {
    return Config.app_id + "/" + Config.dom_id + "/+/+/dev/" + Config.dev_id + "/svc/" + func;

}

void doMqttSubscribe(network::module::mqtt::AClient* aclient) {
    if (!aclient->mqtt.connected())
        return;

    std::string topic;
    topic = getMqttSvcTopic(Config.mqtt_dev_svc_login);
    aclient->mqtt.unsubscribe(topic.c_str()); 
    aclient->mqtt.subscribe(topic.c_str(), 2);

    topic = getMqttSvcTopic(Config.mqtt_dev_svc_handshake);
    aclient->mqtt.unsubscribe(topic.c_str()); 
    aclient->mqtt.subscribe(topic.c_str(), 2);

    topic = getMqttSvcTopic(Config.mqtt_dev_svc_get);
    aclient->mqtt.unsubscribe(topic.c_str()); 
    aclient->mqtt.subscribe(topic.c_str(), 2);

    topic = getMqttSvcTopic(Config.mqtt_dev_svc_set);
    aclient->mqtt.unsubscribe(topic.c_str()); 
    aclient->mqtt.subscribe(topic.c_str(), 2);
}

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

void* onMqttConnect(void* arg, void* p) {
    doMqttSubscribe(GMqttSignaler.mqtt);
    return 0;
}



// void onMqttMessage(MQTTClient *client, char topic[], char bytes[], int length) {
//     service::cmds::Cmd::OnCmd(bytes);
// }

void* OnConfigFixup(void* arg, void* p) {
    cmds::cmd::CmdBase::Command::DefaultFrom->type ="dev";
    cmds::cmd::CmdBase::Command::DefaultFrom->id = Config.dev_id;
    if (Config.edg_id != "") {
        cmds::cmd::CmdBase::Command::DefaultTo->type ="edg";
        cmds::cmd::CmdBase::Command::DefaultTo->id = Config.edg_id;
    }

    cmds::cmd::CmdBase::Command::DefaultRespTimeout = Config.mqtt_resp_timeout;
    cmds::cmd::CmdMqtt::topicPrefix = Config.app_id + "/" + Config.dom_id + "/";
    doMqttSubscribe(GMqttSignaler.mqtt);
    return 0;
}

extern void* OnTestOpen(void* arg, void* p);
void* OnTestClose(void* arg, void* p) {
    std::string code = "4391,4426,573,1616,552,543,573,1616,573,1616,573,520,575,521,573,1617,573,521,573,520,574,1616,573,521,574,521,549,1641,572,1617,571,524,570,1617,575,519,573,1621,569,1618,571,1615,574,1616,549,546,550,1638,572,1617,573,1617,573,521,574,520,550,548,570,521,574,1615,572,523,575,519,573,1617,573,1616,571,1617,574,520,574,521,573,525,570,521,575,519,574,520,575,521,572,522,574,1615,552,1638,573,1625,564,1619,569,1621,568,5266,4481,4432,574,1613,573,523,572,1615,574,1617,571,522,574,520,573,1618,575,517,575,519,575,1618,570,521,578,518,572,1620,569,1618,571,522,573,1615,575,520,574,1616,572,1618,571,1616,573,1618,572,521,573,1616,576,1617,547,1638,573,521,574,520,574,521,574,521,551,1639,576,522,570,520,561,1629,557,1635,569,1617,572,523,571,546,550,526,544,546,572,522,572,522,574,522,571,522,573,1617,572,1627,561,1617,549,1640,573,1617,549,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    GEventTimer.delay(3000, OnTestOpen);
    return 0;
}

void* OnTestOpen(void* arg, void* p) {
    std::string code = "4388,4428,572,1617,571,523,574,1616,571,1617,574,522,574,518,575,1616,549,547,574,519,573,1616,549,547,574,518,574,1615,573,1617,549,546,573,1616,574,520,549,1639,574,522,574,1617,546,1639,572,1619,573,1615,549,1642,573,1615,573,521,572,1617,571,524,572,521,550,553,566,520,573,521,574,1615,573,1616,573,523,573,521,573,522,573,522,571,523,573,521,550,544,574,522,573,1615,573,1617,572,1618,572,1616,576,1612,573,1618,548,5289,4482,4430,571,1617,549,544,551,1640,571,1616,573,525,571,522,573,1616,572,525,570,521,574,1618,572,522,574,519,573,1615,573,1617,549,547,572,1615,574,521,549,1641,571,523,548,1644,569,1615,573,1617,573,1616,558,1631,549,1640,573,522,550,1639,572,523,549,545,573,525,573,521,571,521,583,1606,580,1612,570,525,568,523,549,548,573,519,549,545,550,545,572,522,573,521,550,1639,573,1618,572,1617,572,1616,549,1641,572,1616,549,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    GEventTimer.delay(3000, OnTestClose);
    return 0;
}


void* OnTestOpen26(void* arg, void* p) {
    std::string code = "4419,4431,572,1616,572,522,575,1614,574,1616,573,524,575,515,575,1615,574,520,575,520,574,1615,575,521,571,523,573,1616,574,1613,575,521,574,1614,575,523,572,1614,577,517,575,1615,574,1615,574,1614,573,1615,574,1619,572,1616,574,518,574,1616,574,520,574,520,576,519,576,519,549,545,574,1616,574,1614,574,521,574,1617,572,520,573,521,576,518,575,522,574,520,574,518,577,1616,574,519,574,1615,574,1616,573,1615,574,1615,573,5267,4481,4427,574,1616,574,521,573,1616,574,1614,573,525,571,520,574,1618,571,521,573,524,572,1616,573,525,573,516,574,1617,573,1615,575,520,573,1615,574,521,575,1614,573,522,573,1615,574,1615,573,1618,572,1617,576,1616,569,1619,569,521,575,1616,573,519,575,520,574,529,570,517,575,520,585,1602,584,1607,573,524,570,1617,572,521,574,519,576,521,571,523,574,520,575,521,572,1616,574,520,574,1616,573,1617,572,1615,575,1614,550,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());

    return 0;
}


void* OnTestOpen27(void* arg, void* p) {
    std::string code = "4385,4432,572,1616,573,522,572,1618,571,1617,572,523,549,546,572,1616,573,521,574,521,573,1617,548,545,550,545,571,1620,571,1615,574,521,550,1643,569,523,573,1616,571,521,572,1618,573,1617,572,1618,574,1615,548,1641,573,1615,548,547,548,1640,572,522,572,523,574,521,572,522,570,524,573,1615,554,542,549,549,570,1616,549,546,549,544,550,546,548,546,573,520,573,1620,547,1639,550,545,572,1616,568,1624,547,1640,574,1615,572,5270,4477,4433,570,1616,573,523,571,1619,573,1615,572,522,548,547,549,1640,571,522,550,544,573,1618,573,521,571,524,548,1641,549,1643,568,522,574,1616,549,546,571,1619,572,521,549,1640,548,1650,563,1617,573,1615,553,1641,568,1618,571,526,568,1618,548,545,577,519,571,530,573,513,574,522,584,1605,553,545,569,521,550,1643,568,523,550,545,548,547,572,521,550,545,574,1617,571,1617,572,521,573,1616,573,1616,574,1616,573,1616,573,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());

    return 0;
}


void* OnTestOpen27Auto(void* arg, void* p) {
    std::string code = "4387,4436,572,1615,570,521,573,1617,548,1640,549,547,570,524,573,1615,572,522,553,542,549,1640,582,514,572,521,574,1619,569,1616,573,521,575,1615,573,522,572,522,574,521,572,1617,573,1615,573,1619,572,1616,571,1620,571,1620,568,1616,573,1616,572,522,550,545,573,521,574,521,572,522,571,1620,572,522,548,546,572,1617,575,1615,571,524,572,522,549,545,571,522,574,1617,572,1616,576,517,575,520,573,1616,574,1616,573,1616,576,5263,4484,4429,571,1618,571,523,571,1617,549,1641,548,547,572,522,572,1616,573,520,573,524,571,1616,573,525,570,521,575,1616,573,1616,549,544,550,1648,566,520,574,520,573,521,575,1618,570,1621,568,1618,548,1640,573,1620,569,1617,572,1616,573,1617,572,520,578,523,572,518,550,547,583,507,574,1626,562,523,575,523,571,1616,571,1616,550,547,548,547,570,522,557,538,549,1640,573,1615,572,524,573,521,572,1617,573,1616,573,1617,549,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    return 0;
}


void* OnTestOpen27Cool(void* arg, void* p) {
    std::string code = "4418,4432,571,1616,575,519,574,1616,573,1615,575,524,569,522,573,1616,549,545,550,545,549,1641,572,522,572,522,575,1614,574,1617,554,539,571,1619,572,522,573,1615,553,542,572,1617,574,1616,571,1617,572,1617,549,1640,573,1615,574,521,573,1617,573,521,575,519,575,520,573,522,549,544,551,1639,573,523,572,521,553,1635,574,521,575,520,550,544,574,522,573,521,549,1640,575,1615,572,522,549,1641,571,1618,548,1641,575,1613,573,5266,4488,4424,573,1615,574,521,573,1617,572,1617,548,546,573,522,573,1615,574,521,549,546,548,1640,574,525,569,522,572,1617,549,1640,573,522,571,1619,569,523,574,1617,570,522,574,1616,573,1619,571,1615,553,1637,577,1617,568,1616,572,522,573,1617,571,523,572,523,557,542,572,518,573,528,569,1613,584,511,573,521,572,1621,571,521,573,520,574,522,572,522,549,544,573,1618,549,1639,573,521,574,1634,531,1641,572,1615,575,1616,549,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    return 0;
}

void* OnTestOpen27Dry(void* arg, void* p) {
    std::string code = "4394,4424,579,1608,585,511,584,1605,606,1588,578,511,582,515,581,1607,578,516,579,515,582,1606,580,516,578,516,577,1611,607,1584,577,518,577,1612,579,516,576,519,573,520,574,1620,570,1615,576,1612,575,1615,573,1617,572,1615,574,1615,572,1618,572,523,570,524,575,521,570,523,547,548,546,1645,544,549,549,546,545,1643,571,523,548,1641,545,549,546,549,572,523,546,1643,546,1644,569,524,547,1647,542,549,546,1642,547,1642,573,5264,4454,4460,547,1642,545,551,568,1621,542,1645,545,573,521,550,569,1626,539,549,545,551,545,1644,542,553,543,550,549,1641,543,1646,543,553,542,1646,543,553,540,554,566,529,545,1643,542,1647,545,1645,543,1645,547,1646,539,1646,543,1647,542,1645,545,550,543,551,549,551,557,534,554,541,542,1653,536,553,543,554,540,1646,550,557,529,1669,520,553,565,551,546,527,542,1647,541,1671,542,531,540,1648,540,555,566,1623,539,1650,540,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    return 0;
}

void* OnTestOpen27Heat(void* arg, void* p) {
    std::string code = "4390,4428,571,1617,549,546,548,1646,568,1617,572,520,574,521,574,1619,546,546,549,545,572,1617,549,545,573,522,572,1617,573,1616,573,522,572,1618,571,522,574,1615,573,522,574,1615,549,1641,570,1619,571,1616,578,1612,574,1618,570,524,571,1617,571,522,570,524,573,522,573,522,572,522,573,1617,571,524,572,523,571,1618,575,1613,572,1617,573,521,575,519,575,522,548,1640,571,1617,549,544,574,522,574,521,573,1618,547,1640,574,5265,4481,4432,571,1616,573,521,573,1618,571,1617,572,523,572,525,570,1617,552,542,573,525,578,1608,572,521,574,521,550,1640,572,1617,575,522,569,1617,574,520,573,1618,570,524,572,1616,550,1640,572,1616,573,1617,570,1622,569,1618,571,522,572,1617,573,521,573,522,573,530,545,542,572,521,586,1607,554,541,570,522,571,1622,569,1616,571,1617,549,546,549,545,572,523,549,1641,548,1640,574,521,572,521,551,546,570,1617,574,1616,571,65535";
    rfir::GetRfir()->sender->sendRaw(code.c_str(), code.size());
    return 0;
}




void* OnTest(void* arg, void* p) {
    OnTestOpen27Auto(0, 0);
    delay(1000);
    OnTestOpen27Cool(0, 0);
    delay(1000);
    OnTestOpen27Dry(0, 0);
    delay(1000);
    OnTestOpen27Heat(0, 0);
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
    //SMC
    #ifdef SMC_MODE
    np.smcMode = SMC_MODE;
    np.smc.configVersion = SMC_CONFIG_VERSION;
    np.smc.configFile = SMC_CONFIG_FILE; 
    np.smc.configPin = SMC_CONFIG_PIN;  
    np.smc.configPinType = SMC_CONFIG_PIN_TYPE;
    np.smc.configPinNum = SMC_CONFIG_PIN_NUM;
    np.smc.configPinDelay = SMC_CONFIG_PIN_DELAY;
    np.smc.configPinInterval = SMC_CONFIG_PIN_INTERVAL;
    np.smc.configTimeout = SMC_CONFIG_TIMEOUT;
    #endif    

    network::service::wifi::Client::Start(np);

    #ifdef WIFI_SSID_DEV
    np.ssid.push_back(WIFI_SSID_DEV);
    np.pass.push_back(WIFI_PASSWORD_DEV);    
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
    GCmdDispatcher.events.onConnect.add((void*)&onMqttConnect, onMqttConnect);

    network::service::mqtt::AClient::Start(mp);
#endif

    //启动收发器
    rfir::Start(onRfirStart, onRfirSniffed, onRfirDecoded, onRfirEncoded, onRfirSended);

    //启动 设备
    rfir::service::device::Device::Start(onDeviceChange);
    

    //业务开始
    service::cmds::Cmd::Start();

    //定时器
    GEventTimer.start();
    //全局配置
    Config.events.onFixup.add((void*)&OnConfigFixup, OnConfigFixup, 0);
    Config.fixup();

    //测试
    // GEventTimer.delay(3000, OnTest);

}



void loop() {
#if !(defined(DISABLE_WIFI) && DISABLE_WIFI == TRUE)    
    //wifi循环
    network::service::wifi::Client::Loop();
#endif

#if !(defined(DISABLE_OTA) && DISABLE_OTA == TRUE)
    //OTA循环
    network::service::ota::Updater::Loop();
#endif

#if !(defined(DISABLE_MQTT) && DISABLE_MQTT == TRUE)
    //mqtt循环
    network::service::mqtt::AClient::Loop();
#endif    

    //收发器循环
    rfir::Loop();  

    //业务循环
    service::cmds::Cmd::Loop();

    //定时器
    GEventTimer.loop();
}


