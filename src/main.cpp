#include <Arduino.h>
#include "config.h"
#include <IotWebConf.h>
#include "network/module/wifi/client.h"
#include "network/module/ota/updater.h"
#include "network/service/wifi/client.h"
#include "network/service/mqtt/client.h"
#include "network/service/ota/updater.h"


#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/cmds/cmd.h"
#include "rfir/service/device/device.h"
#include "rfir/rfir.h"
#include "rfir/module/ttl/device/mcquay.h"

#include "service/cmds/cmd.h"


std::string ChipID = rfir::util::Util::GetChipId(CHIP_ID_PREFIX);

void onRfirSniffed(rfir::module::ttl::Sniffer* sniffer, rfir::module::ttl::Delta* data, int count) {
    Serial.println("onRfirSniffed");
    Serial.println(sniffer->toString());
    
    // std::string str = rfir::module::ttl::Sniffer::packSniffedCmd(sniffer, sniffer->toString().c_str());
    // Serial.println(str.c_str());  
    // Serial.println("");  

    // if (sniffer->getSniffParams()->response) {
    //     if (network::service::mqtt::Client::Publish(str.c_str())) {
    //         Serial.println("mqtt 发送成功");

    //     } else {
    //         Serial.println("mqtt 发送失败");
    //     }
    // } else {
    //     Serial.println("sniffer->getSniffParams()->response: false");
    // }
    
}

void onRfirDecoded(rfir::module::ttl::Decoder* decoder, rfir::module::ttl::Decoder::DecodeResults* data) {
    Serial.println("onRfirDecoded");
    Serial.println(data->toJsonString().c_str());
    Serial.println(data->toJsonString(true).c_str());

    // std::string str = rfir::module::ttl::Decoder::packDecodedCmd(decoder, data);
    // Serial.println(str.c_str()); 
    // Serial.println("");

    if (data->count > 0) 
        service::cmds::Cmd::OnCmd_decoded(data);
    return;


    // if (decoder->getDecodeParams()->response) {
    //     network::service::mqtt::Client::Publish(str.c_str());
    // } else {
    //     Serial.println("sniffer->getDecodeParams()->response: false");
    // }
    
    // neb::CJsonObject cmd, pld, jdecode, blocks;
    // cmd.Parse(str);
    // cmd.Get("pld", pld);
    // pld.Get("decode", jdecode);
    // jdecode.Get("blocks", blocks);
    // decoder->rfir->encoder->getEncodeParams()->clone(decoder->getDecodeParams());
    // decoder->rfir->encoder->encode(data);
}

void onRfirEncoded(rfir::module::ttl::Encoder* encoder, rfir::module::ttl::Encoder::EncodeResult* data) {
    Serial.println("onRfirEncoded: " + String(data->count));
    Serial.println(data->toString());
    // std::string str = rfir::module::ttl::Encoder::packEncodedCmd(encoder, data);
    // Serial.println(str.c_str()); 
    // Serial.println(""); 
    // if (encoder->getEncodeParams()->response) {
    //     network::service::mqtt::Client::Publish(str.c_str());
    // }  else {
    //     Serial.println("encoder->getEncodeParams()->response: false");
    // }   

    // encoder->rfir->sender->sendRaw(data->result, data->count);




    // auto rfir = rfir::Get()->getRfir("38_gree", false);
    // if (rfir) {
    //     Serial.println(rfir->sender->getSendParams()->toString().c_str());
    //     rfir->sender->sendRaw(data->result, data->count);
    // } else {
    //     Serial.println("没有设备：38_gree");
    // }    
}

void onRfirSended(rfir::module::ttl::Sender* sender, const uint16_t* data, const uint16_t len) {
    Serial.println("onRfirSended: " + String(len));
    std::string str = rfir::module::ttl::Sender::packSendedCmd(sender, data, len);
    Serial.println(str.c_str());  
    // Serial.println("");
    // if (sender->getSendParams()->response) {
    //     if (network::service::mqtt::Client::Publish(str.c_str())) {
    //         Serial.println("mqtt 发送成功");

    //     } else {
    //         Serial.println("mqtt 发送失败");
    //     }
    // } else {
    //     Serial.println("sender->getSendParams()->response: false");
    // }
    
}

void onDeviceChange(void* device, const char* reason) {
    Serial.printf("onDeviceChange: %s\n", reason);
    service::cmds::Cmd::OnCmd_get(0, reason);
}

void onRfirStart(void* data) {
    Serial.println("onRfirStart");

    //初始化设备
    auto d = rfir::service::device::Device::Init();
    rfir::service::cmds::Cmd::onCmd_sniff(d);
    rfir::service::cmds::Cmd::onCmd_send(d);
}

uint16_t onMqttConnect_count = 0;
void onMqttConnect(network::module::mqtt::Client::MQTT* mqtt) {
    onMqttConnect_count++;
    Serial.println("onMqttConnect");
    if (onMqttConnect_count > 1)
        service::cmds::Cmd::OnCmd_heartbeat(0, 1);
    else 
        service::cmds::Cmd::OnCmd_heartbeat(0, 2);
    service::cmds::Cmd::DoTimerReport(true);
}

void onMqttMessage(MQTTClient *client, char topic[], char bytes[], int length) {
    service::cmds::Cmd::OnCmd(bytes);
}


void setup() {
    Serial.begin(115200);
    Serial.println("begin chid id: " + String(ChipID.c_str()));

    //启动wifi
    network::module::wifi::Client::Params np;
    np.ssid = WIFI_SSID; np.pass = WIFI_PAWD;
    network::service::wifi::Client::Start(np);

    //启动OTA
    network::module::ota::Updater::Params op;
    op.url = OTA_UPDATE_URL;
    op.id = ChipID;
    op.version = OTA_VERSION_NUMBER;
    network::service::ota::Updater::Start(op);

    //启动mqtt
    network::module::mqtt::Client::Params mp;
    mp.ip = MQTT_IP;
    mp.port = MQTT_PORT;
    mp.user = MQTT_USER;
    mp.sub_topic = MQTT_SUB_TOPIC + ChipID;
    mp.pub_topic = MQTT_PUB_TOPIC;
    mp.bufsize = 2 * 1024;
    mp.id = ChipID;
    network::service::mqtt::Client::Start(mp, onMqttConnect, onMqttMessage);

    //启动收发器
    rfir::Start(onRfirStart, onRfirSniffed, onRfirDecoded, onRfirEncoded, onRfirSended);

    //启动 设备
    rfir::service::device::Device::Start(onDeviceChange);
    
    //业务开始
    service::cmds::Cmd::Start();
}



void loop() {
    //wifi循环
    network::service::wifi::Client::Loop();
    //OTA循环
    network::service::ota::Updater::Loop();
    //mqtt循环
    network::service::mqtt::Client::Loop();

    //收发器循环
    rfir::Loop();  

    //业务循环
    service::cmds::Cmd::Loop();

}


