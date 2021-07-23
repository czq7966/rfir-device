#include "sniffer.h"
#include "decoder.h"
#include "config.h"
#include "../serial/sender.h"
#include "../../module/cmds/cmd.h"
#include "../../module/ttl/decoder.h"
#include "../../module/ttl/config.h"


rfir::module::ttl::Decoder* rfir::service::ttl::Decoder::ttldecoder = new rfir::module::ttl::Decoder();


void rfir::service::ttl::Decoder::decode(rfir::module::ttl::Decoder* decoder) {
    auto sniffer = rfir::service::ttl::Sniffer::ttlsniffer;
    if (!decoder) decoder = rfir::service::ttl::Decoder::ttldecoder;    

    int count = sniffer->sniffDelta();
    if (count > 0) {
        auto data = (uint16_t*)(sniffer->getSniffedDelta());
        //rfir::module::ttl::Decoder::Params p1;
        //p1.use_bits = p1.nbits % 8;

        //rfir::module::ttl::Decoder::Params* p = rfir::service::ttl::Config::ttlconfig->device->packet.decode.params;
        //int count = rfir::service::ttl::Config::ttlconfig->device->packet.decode.count;

        //Serial.println(p->toString().c_str());
        //Serial.println((p + 1)->toString().c_str());
        //p->MSBfirst = true;
        //p1.MSBfirst = false;

        

        //decoder->setDecodeParams(p, 3);
        //decoder->setDecodeParams(&p1, 1);
        int offset = 0;
        while (offset < count) {
            int matched = decoder->decode(data + offset, count);
            if (!matched) 
                break;

            offset += matched;
        }
    
        sniffer->resetSniff();    
    }
}