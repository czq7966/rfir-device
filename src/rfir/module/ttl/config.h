#ifndef __RFIR_MODULE_TTL_CONFIG_H__
#define __RFIR_MODULE_TTL_CONFIG_H__


#include "../../util/platform.h"
#include "../../util/cjson/CJsonObject.hpp"
#include "../../util/file.h"
#include "sniffer.h"
#include "decoder.h"
#include "sender.h"
#include "encoder.h"

#define TTL_DEFAULT_CONFIG_STR "{\"gree\":{\"sniff\":{\"params\":{\"pin\":22,\"mode\":5,\"inverted\":false,\"minCount\":50,\"minDelta\":150,\"maxDelta\":65535,\"bufSize\":4096}},\"decode\":{\"blocks\":[{\"params\":{\"tolerance\":20,\"excess\":0,\"atleast\":true,\"MSBfirst\":false,\"nbits\":32,\"headermark\":9000,\"headerspace\":4500,\"onemark\":620,\"onespace\":1600,\"zeromark\":620,\"zerospace\":540,\"footermark\":0,\"footerspace\":0}},{\"params\":{\"tolerance\":20,\"excess\":0,\"atleast\":true,\"MSBfirst\":false,\"nbits\":3,\"headermark\":0,\"headerspace\":0,\"onemark\":620,\"onespace\":1600,\"zeromark\":620,\"zerospace\":540,\"footermark\":620,\"footerspace\":19000}},{\"params\":{\"tolerance\":20,\"excess\":0,\"atleast\":true,\"MSBfirst\":false,\"nbits\":32,\"headermark\":0,\"headerspace\":0,\"onemark\":620,\"onespace\":1600,\"zeromark\":620,\"zerospace\":540,\"footermark\":620,\"footerspace\":19000}}]},\"send\":{\"params\":{\"pin\":23,\"inverted\":false,\"modulation\":true,\"repeat\":1,\"MSBfirst\":true,\"frequency\":38,\"dutycycle\":50}}}}"


namespace rfir {
    namespace module {
        namespace ttl {
           class Config {
            public:
                struct Packet {
                    rfir::module::ttl::Sniffer::SniffParams   sniff;
                    rfir::module::ttl::Decoder::DecodeParams  decode;
                    rfir::module::ttl::Sender::SendParams     send;
                    rfir::module::ttl::Encoder::EncodeParams  encode;
                };

                struct Device {
                    std::string name;
                    Packet packet;
                };

                struct Devices {
                    Device* device = 0;
                    int     count = 0;
                };

            public:
                static String ConfigFilename;
            public:
                Config();
                ~Config();
                bool init();
                bool init(neb::CJsonObject* jCfg);
                
                void uninit();


            public:
                bool changeConfig(neb::CJsonObject& jCfg, bool save = false);
                bool changeConfig(String& strCfg, bool save = false);
                bool changeConfigFromFile(String fn);
                bool loadConfig();


            public:
                Devices devices;
                neb::CJsonObject jDevices;
            public:
                static bool initDevices(neb::CJsonObject* jdevices, Devices* devices);
                static bool initDevice(neb::CJsonObject* jdevice, Device* device);
                static bool initDeviceSniff(neb::CJsonObject* jsniff, rfir::module::ttl::Sniffer::SniffParams* sniff);
                static bool initDeviceDecode(neb::CJsonObject* jdecode, rfir::module::ttl::Decoder::DecodeParams* decode);                
                static bool initDeviceSend(neb::CJsonObject* jsend, rfir::module::ttl::Sender::SendParams* send);
                static bool initDeviceEncode(neb::CJsonObject* jencode, rfir::module::ttl::Encoder::EncodeParams* encode);

                static bool uninitDevices(Devices* devices);
                static bool uninitDevice(Device* device);
                static bool uninitDeviceDecode(rfir::module::ttl::Decoder::DecodeParams* decode);
                static bool uninitDeviceEncode(rfir::module::ttl::Encoder::EncodeParams* encode);
            };
        }
    }
}

#endif //__RFIR_MODULE_TTL_CONFIG_H__