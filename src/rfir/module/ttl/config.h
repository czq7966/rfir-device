#ifndef __RFIR_MODULE_TTL_CONFIG_H__
#define __RFIR_MODULE_TTL_CONFIG_H__

#include <vector>

#include "../../util/platform.h"
#include "../../util/cjson/CJsonObject.hpp"
#include "../../util/file.h"
#include "config-default.h"
#include "sniffer.h"
#include "decoder.h"
#include "sender.h"
#include "encoder.h"




namespace rfir {
    namespace module {
        namespace ttl {
            class RFIR;
            class Config {
            public:
                struct Packet {
                    Sniffer::SniffParams   sniff;
                    Decoder::DecodeParams  decode;
                    Sender::SendParams     send;
                    Encoder::EncodeParams  encode;
                    void    free();
                    bool    parseFromJson(neb::CJsonObject* jp);  
                    bool    clone(Packet* packet);
                };

                struct Device {
                    std::string*  name = 0;
                    Packet  packet;
                    void    free();
                    bool    parseFromJson(neb::CJsonObject* jp, const char* name = 0);  
                    bool    clone(Device* device);
                    std::string getName();
                    void    setName(const char* name);
                };

                struct Devices {
                    std::vector<Device*>    device;

                    void                free();
                    bool                clone(Devices* devices);
                    Device*             getDevice(std::string name = "");
                    Device*             newDevice(std::string name = "");
                    int                 getCount();

                    bool                parseFromJson(neb::CJsonObject* jp);                    

                };

            public:
                Config(RFIR* rfir = 0);
                ~Config();
                bool init(std::string fn = "");
                void uninit();

                Device* getDevice(std::string name);
                bool    loadFromFile(std::string fn);
                bool    loadFromJson(neb::CJsonObject * jdevices);
                bool    loadFromString(const char* str);

            public:
                RFIR*   rfir = 0;
                Devices devices;
            };
        }
    }
}

#endif //__RFIR_MODULE_TTL_CONFIG_H__