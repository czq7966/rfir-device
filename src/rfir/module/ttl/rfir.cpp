#include "rfir.h"

rfir::module::ttl::RFIR::RFIR() {
    init();
}

rfir::module::ttl::RFIR::~RFIR() {
    uninit();
}

void rfir::module::ttl::RFIR::init() {
    this->config = new Config(this);
    this->sniffer = new Sniffer(this);
    this->decoder = new Decoder(this);
    this->encoder = new Encoder(this);
    this->sender = new Sender(this);
}

void rfir::module::ttl::RFIR::uninit() {
    delete this->sender;
    delete this->encoder;
    delete this->decoder;
    delete this->sniffer;
    delete this->config;

    this->sender = 0;
    this->encoder = 0;
    this->decoder = 0;
    this->sniffer = 0;
    this->config = 0;

}

