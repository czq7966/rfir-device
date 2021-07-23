#include "sniffer.h"
#include "../serial/sender.h"
#include "../../module/cmds/cmd.h"
#include "../../module/ttl/sniffer.h"


rfir::module::ttl::Sniffer* rfir::service::ttl::Sniffer::ttlsniffer = new rfir::module::ttl::Sniffer();

void rfir::service::ttl::Sniffer::init(rfir::module::ttl::Sniffer::Params params) {
    ttlsniffer->init(params);
}



int rfir::service::ttl::Sniffer::sniff() {
    int count = ttlsniffer->sniffDelta();
    if(count > 0) {       
       ttlsniffer->resumeSniff();     
       auto data = ttlsniffer->getSniffedDelta();
       String str;
       //str = data[0].ttl ? "1,": "0,";
       //str = str + String(data[0].delta) +",";
       for (size_t i = 0; i < count; i++)
       {
           str = str + String(data[i].delta) + ",";
       }       

        //rfir::service::serial::Sender::pushCmd(rfir::module::cmd::CmdId::sniffed.c_str(), str.c_str());

        Serial.println(str.c_str());
    }
    return count;
}


void rfir::service::ttl::Sniffer::start() {
    attachInterrupt(ttlsniffer->gpio.pin, gpio_interrupt, CHANGE);
}

void rfir::service::ttl::Sniffer::stop() {
    detachInterrupt(ttlsniffer->gpio.pin);
}

void rfir::service::ttl::Sniffer::gpio_interrupt() {
    int value = ttlsniffer->gpio.read();
    rfir::service::ttl::Sniffer::ttlsniffer->pushTTL(value);
}

