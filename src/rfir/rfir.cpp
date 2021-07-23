#include "rfir.h"
#include "util/file.h"


void rfir::RFIR::start(const char* fn, 
                        rfir::module::ttl::Sniffer::OnSniffed onSniffed, 
                        rfir::module::ttl::Decoder::OnDecoded onDecoded,
                        rfir::module::ttl::Encoder::OnEncoded onEncoded) {
    if (!SPIFFS.begin())
        Serial.println("error::SPIFFS文件系统挂载失败！");

    if (fn)
        rfir::module::ttl::Config::ConfigFilename = fn;

    if (rfir::service::ttl::Config::ttlconfig->init())
        Serial.println("配置初始化成功");
    rfir::service::ttl::Sniffer::ttlsniffer->onSniffed = onSniffed;
    rfir::service::ttl::Decoder::ttldecoder->onDecoded = onDecoded;
    rfir::service::ttl::Encoder::ttlencoder->onEncoded = onEncoded;

}
void rfir::RFIR::loop() {
  rfir::service::serial::Receiver::dealCmd();
  rfir::service::ttl::Decoder::decode();
  rfir::service::serial::Sender::dealCmd();
}
