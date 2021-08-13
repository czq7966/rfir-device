#ifndef __RFIR_MODULE_TTL_CONFIG_H__
#define __RFIR_MODULE_TTL_CONFIG_H__


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
                };

                struct Devices {
                    Device*             device = 0;
                    int                 count = 0;

                    void                free();
                    bool                parseFromJson(neb::CJsonObject* jp);
                    Device*             getDevice(std::string name);
                    bool                clone(Devices* devices);
                    // std::list<Device*>  device;
                    // Device*             addDevice(std::string name);
                    // int                 getCount();
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