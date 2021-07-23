#include <Arduino.h>

#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"
#include "rfir/service/ttl/sniffer.h"
#include "rfir/service/ttl/decoder.h"
#include "rfir/service/ttl/encoder.h"
#include "rfir/service/cmds/cmd.h"
#include "rfir/rfir.h"



void onSniffed(rfir::module::ttl::Sniffer* sniffer, rfir::module::ttl::Delta* data, int count) {
  std::string str = rfir::module::ttl::Sniffer::packSniffedCmd(&(sniffer->params), sniffer->toString().c_str());
  Serial.println(str.c_str());  
}

void onDecoded(rfir::module::ttl::Decoder* decoder, rfir::module::ttl::Decoder::DecodeResults* data, String name) {
  std::string str = rfir::module::ttl::Decoder::packDecodedCmd(decoder, data, name);
  Serial.println(str.c_str());  

  neb::CJsonObject cmd, pld, jdecode, blocks;
  cmd.Parse(str);
  cmd.Get("pld", pld);
  pld.Get("decode", jdecode);
  jdecode.Get("blocks", blocks);

  rfir::service::ttl::Encoder::ttlencoder->setJEncode(&jdecode);
  rfir::service::ttl::Encoder::ttlencoder->encode(&blocks);
  
}

void onEncoded(rfir::module::ttl::Encoder* encoder, rfir::module::ttl::Encoder::EncodeResult* data, String name) {
  std::string str = rfir::module::ttl::Encoder::packEncodedCmd(encoder, data, name);
  Serial.println(str.c_str());  
}


void setup() {
  Serial.begin(115200);
  Serial.println("begin");

  rfir::start(onSniffed, onDecoded, onEncoded);

}

void loop() {
  rfir::loop();

}