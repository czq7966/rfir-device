#ifndef __RFIR_RFIR_H__
#define __RFIR_RFIR_H__

#include "util/platform.h"
#include "util/file.h"
#include "module/ttl/sniffer.h"
#include "module/ttl/decoder.h"
#include "module/ttl/sender.h"
#include "module/ttl/encoder.h"
#include "module/ttl/config.h"
#include "module/serial/receiver.hpp"
#include "module/serial/sender.hpp"
#include "service/ttl/sniffer.h"
#include "service/ttl/decoder.h"
#include "service/ttl/sender.h"
#include "service/ttl/encoder.h"
#include "service/ttl/config.h"
#include "service/serial/receiver.h"
#include "service/serial/sender.h"



namespace rfir {
    class RFIR {   
    public:
        rfir::module::ttl::Sniffer sniffer;
        rfir::module::ttl::Decoder decoder;
        rfir::module::ttl::Sender  sender;
        rfir::module::ttl::Encoder encoder;

    public:
        static void start(const char* fn = 0, 
                            rfir::module::ttl::Sniffer::OnSniffed onSniffed = 0, 
                            rfir::module::ttl::Decoder::OnDecoded onDecoded = 0,
                            rfir::module::ttl::Encoder::OnEncoded onEncoded = 0);
        static void loop();      
    };

    static void start(const char* fn, 
                        rfir::module::ttl::Sniffer::OnSniffed onSniffed = 0, 
                        rfir::module::ttl::Decoder::OnDecoded onDecoded = 0,
                        rfir::module::ttl::Encoder::OnEncoded onEncoded = 0) {
        RFIR::start(fn, onSniffed, onDecoded, onEncoded);
    };

    static void start(rfir::module::ttl::Sniffer::OnSniffed onSniffed = 0, 
                        rfir::module::ttl::Decoder::OnDecoded onDecoded = 0,
                        rfir::module::ttl::Encoder::OnEncoded onEncoded = 0
                        ) {
        start(0, onSniffed, onDecoded, onEncoded);
    };    

    static void loop() {
        RFIR::loop();
    }
}

#endif