#include <Arduino.h>
#include <IotWebConf.h>
#include "network/module/wifi/client.h"
#include "network/service/wifi/client.h"
#include "network/service/mqtt/client.h"

#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/cmds/cmd.h"
#include "rfir/rfir.h"

#include "rfir/module/ttl/device/gree.h"


void onSniffed(rfir::module::ttl::Sniffer* sniffer, rfir::module::ttl::Delta* data, int count) {
    std::string str = rfir::module::ttl::Sniffer::packSniffedCmd(sniffer, sniffer->toString().c_str());
    Serial.println(str.c_str());  
    Serial.println("");  

    if (sniffer->getSniffParams()->response) {
        if (network::service::mqtt::Client::Publish(str.c_str())) {
            Serial.println("mqtt 发送成功");

        } else {
            Serial.println("mqtt 发送失败");
        }
    } else {
        Serial.println("sniffer->getSniffParams()->response: false");
    }
    
}

void onDecoded(rfir::module::ttl::Decoder* decoder, rfir::module::ttl::Decoder::DecodeResults* data) {
    std::string str = rfir::module::ttl::Decoder::packDecodedCmd(decoder, data);
    Serial.println(str.c_str()); 
    Serial.println("");
    if (decoder->getDecodeParams()->response) {
        network::service::mqtt::Client::Publish(str.c_str());
    } else {
        Serial.println("sniffer->getDecodeParams()->response: false");
    }
    
    // neb::CJsonObject cmd, pld, jdecode, blocks;
    // cmd.Parse(str);
    // cmd.Get("pld", pld);
    // pld.Get("decode", jdecode);
    // jdecode.Get("blocks", blocks);
    // decoder->rfir->encoder->getEncodeParams()->clone(decoder->getDecodeParams());
    decoder->rfir->encoder->encode(data);
}

void onEncoded(rfir::module::ttl::Encoder* encoder, rfir::module::ttl::Encoder::EncodeResult* data) {
    std::string str = rfir::module::ttl::Encoder::packEncodedCmd(encoder, data);
    Serial.println(str.c_str()); 
    Serial.println(""); 
    if (encoder->getEncodeParams()->response) {
        network::service::mqtt::Client::Publish(str.c_str());
    }  else {
        Serial.println("encoder->getEncodeParams()->response: false");
    }   

    encoder->rfir->sender->sendRaw(data->result, data->count);




    // auto rfir = rfir::Get()->getRfir("38_gree", false);
    // if (rfir) {
    //     Serial.println(rfir->sender->getSendParams()->toString().c_str());
    //     rfir->sender->sendRaw(data->result, data->count);
    // } else {
    //     Serial.println("没有设备：38_gree");
    // }    
}

void onSended(rfir::module::ttl::Sender* sender, const uint16_t* data, const uint16_t len) {
    std::string str = rfir::module::ttl::Sender::packSniffedCmd(sender, data, len);
    Serial.println(str.c_str());  
    Serial.println("");
    if (sender->getSendParams()->response) {
        if (network::service::mqtt::Client::Publish(str.c_str())) {
            Serial.println("mqtt 发送成功");

        } else {
            Serial.println("mqtt 发送失败");
        }
    } else {
        Serial.println("sender->getSendParams()->response: false");
    }
    
}

void onStart(void* data) {
    Serial.println("onStart");

    auto d = rfir::module::ttl::device::Gree::Init(&rfir::RFIR::Config->devices);
#ifdef ESP8266
    //采码参数
    d->packet.sniff.params.pin = 5;
    d->packet.sniff.params.bufSize = 512;
    //发码参数 
    d->packet.send.params.pin = 2;
    d->packet.send.params.repeat = 1;
#else
    //采码参数
    d->packet.sniff.params.pin = 22;
    d->packet.sniff.params.bufSize = 512;
    //发码参数 
    d->packet.send.params.pin = 23;
    d->packet.send.params.repeat = 1;
#endif    

    rfir::service::cmds::Cmd::onCmd_sniff(d);
    rfir::service::cmds::Cmd::onCmd_send(d);

}


void setup() {
    Serial.begin(115200);
    Serial.println("begin");

    //启动wifi
    network::module::wifi::Client::Params np;
    np.ssid = "AX6000"; np.pass = "77777777";
    // network::service::wifi::Client::Start(np);

    //启动mqtt
    network::module::mqtt::Client::Params mp;
    mp.ip = "172.24.140.41";
    mp.port = 1883;
    mp.user = "device";
    mp.sub_topic = "cmdgate/device/" + rfir::util::Util::GetChipId();
    mp.pub_topic = "cmdgate/central/cmd";
    mp.bufsize = 3 * 1024;
    // network::service::mqtt::Client::Start(mp);

    //启动收发器
    rfir::Start(onStart, onSniffed, onDecoded, onEncoded, onSended);

}



void loop() {
    //wifi循环
    network::service::wifi::Client::Loop();
    //mqtt循环
    network::service::mqtt::Client::Loop();
    //收发器循环
    rfir::Loop();  
}


