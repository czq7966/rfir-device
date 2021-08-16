#ifndef __RFIR_RFIR_H__
#define __RFIR_RFIR_H__

#include "module/ttl/rfir.h"
#include <map>

namespace rfir {
    class RFIR {   
    private:
         std::map<std::string,  rfir::module::ttl::RFIR*> rfirMap;
    public:
        void loop();   
        rfir::module::ttl::RFIR* getRfir(std::string name, bool create = true);

    public:
        static std::map<std::string, RFIR*>* RfirMap;
    public:        
        static std::string ConfigFile;
        static rfir::module::ttl::Config* Config;
        static std::function<void(void *)> OnStart;
        static rfir::module::ttl::Sniffer::OnSniffed OnSniffed; 
        static rfir::module::ttl::Decoder::OnDecoded OnDecoded;
        static rfir::module::ttl::Encoder::OnEncoded OnEncoded;        
        static rfir::module::ttl::Sender::OnSended   OnSended;
        static RFIR* Get(std::string name);
        static void Loop();
        static bool RefreshConfig();
        static bool RefreshConfig(const char* content);
        static void StopSniff();
        static void Start();
        static void Stop();
    };

    


    static void Start(const char* fn, 
                        std::function<void(void *)> onStart = 0,
                        rfir::module::ttl::Sniffer::OnSniffed onSniffed = 0, 
                        rfir::module::ttl::Decoder::OnDecoded onDecoded = 0,
                        rfir::module::ttl::Encoder::OnEncoded onEncoded = 0,
                        rfir::module::ttl::Sender::OnSended   onSended = 0
                        ) {
        rfir::RFIR::ConfigFile = fn ? fn : rfir::RFIR::ConfigFile;
        rfir::RFIR::OnStart = onStart;
        rfir::RFIR::OnSniffed = onSniffed;
        rfir::RFIR::OnDecoded = onDecoded;
        rfir::RFIR::OnEncoded = onEncoded;
        rfir::RFIR::OnSended  = onSended;
        rfir::RFIR::Config->init(rfir::RFIR::ConfigFile);
        rfir::RFIR::Start();
    };

    static void Start(  std::function<void(void *)> onStart = 0,
                        rfir::module::ttl::Sniffer::OnSniffed onSniffed = 0, 
                        rfir::module::ttl::Decoder::OnDecoded onDecoded = 0,
                        rfir::module::ttl::Encoder::OnEncoded onEncoded = 0,
                        rfir::module::ttl::Sender::OnSended   onSended = 0
                        ) {
        Start(0, onStart, onSniffed, onDecoded, onEncoded, onSended);
    };    

    static void Loop() {
        rfir::RFIR::Loop();
    }

    static RFIR* Get() {
        return rfir::RFIR::Get("");
    }

    static rfir::module::ttl::Config::Device* getRfirDevice(std::string name) {
        auto device = rfir::RFIR::Config->getDevice(name);
        if (!device) {
            Serial.println(("RFIR->getRfirDevice-> no Config device:" + name).c_str());
            return 0;
        }

        auto rfir = rfir::Get()->getRfir(device->getName());
        device = rfir->config->getDevice(device->getName());        
        if (!device)
            Serial.println(("RFIR->getRfirDevice-> no config device:" + name).c_str());
        return device;
    }


}

#endif