#include "rfir.h"
#include "util/file.h"
#include "rfir/service/serial/receiver.h"
#include "rfir/service/serial/sender.h"

std::map<std::string, rfir::RFIR*>* rfir::RFIR::RfirMap = new std::map<std::string, rfir::RFIR*>();

std::string rfir::RFIR::ConfigFile = "/rfir-config.json";
rfir::module::ttl::Config* rfir::RFIR::Config = new rfir::module::ttl::Config();
std::function<void(void *)> rfir::RFIR::OnStart = 0;
rfir::module::ttl::Sniffer::OnSniffed rfir::RFIR::OnSniffed = 0;
rfir::module::ttl::Decoder::OnDecoded rfir::RFIR::OnDecoded = 0;
rfir::module::ttl::Encoder::OnEncoded rfir::RFIR::OnEncoded = 0;
rfir::module::ttl::Sender::OnSended rfir::RFIR::OnSended = 0;

rfir::RFIR* rfir::RFIR::Get(std::string name) {
    auto rfir = (*RfirMap)[name];
    if (!rfir) {
        rfir = new RFIR();
        (*RfirMap)[name] = rfir;
    }

    return rfir;
}

void rfir::RFIR::Loop() {
    rfir::service::serial::Receiver::dealCmd();
    for (auto it = RfirMap->begin(); it != RfirMap->end(); it++)
    {        
        it->second->loop();
    }
    rfir::service::serial::Sender::dealCmd();
}

bool rfir::RFIR::RefreshConfig() {
    rfir::RFIR::Config->init(rfir::RFIR::ConfigFile);
    for (auto it = RfirMap->begin(); it != RfirMap->end(); it++)
    {   
        auto Rfir = it->second;
        
        for (auto it1 = Rfir->rfirMap.begin(); it1 !=  Rfir->rfirMap.end(); it1++)
        {   
            auto rfir = it1->second;
            rfir->config->init(rfir::RFIR::ConfigFile);
        }        
        
    }
}

bool rfir::RFIR::RefreshConfig(const char* content) {
    rfir::RFIR::Config->loadFromString(content);
    for (auto it = RfirMap->begin(); it != RfirMap->end(); it++)
    {    
        auto Rfir = it->second;
        
        for (auto it1 = Rfir->rfirMap.begin(); it1 !=  Rfir->rfirMap.end(); it1++)
        {   
            auto rfir = it1->second;
            rfir->config->loadFromString(content);
        }        

    }
}

void rfir::RFIR::StopSniff() {
    for (auto it = RfirMap->begin(); it != RfirMap->end(); it++)
    {  
        auto Rfir = it->second;
        
        for (auto it1 = Rfir->rfirMap.begin(); it1 !=  Rfir->rfirMap.end(); it1++)
        {   
            auto rfir = it1->second;
            rfir->sniffer->stop();
        }  
    }
}

void rfir::RFIR::Start() {
    if (OnStart)
        OnStart(0);
}

void rfir::RFIR::Stop() {
    StopSniff();
}

void rfir::RFIR::loop() {        
    for (auto it = rfirMap.begin(); it != rfirMap.end(); it++)
    {   
        auto rfir = it->second;
        rfir->sniffer->start();
        rfir->decoder->decode(); 
    }    
}

rfir::module::ttl::RFIR* rfir::RFIR::getRfir(std::string name, bool create) {
    auto Rfir = rfir::Get();
    rfir::module::ttl::RFIR* rfir = 0;
    auto it = Rfir->rfirMap.find(name);
    if (it != Rfir->rfirMap.end()) 
        rfir = it->second;
    
    if (!rfir && create) {

        rfir = new rfir::module::ttl::RFIR();
        rfir->sniffer->onSniffed = rfir::RFIR::OnSniffed;
        rfir->decoder->onDecoded = rfir::RFIR::OnDecoded;
        rfir->encoder->onEncoded = rfir::RFIR::OnEncoded;
        rfir->sender->onSended   = rfir::RFIR::OnSended;
        //rfir->config->init(rfir::RFIR::ConfigFile);   
        rfir->config->devices.clone(&rfir::RFIR::Config->devices);
        Rfir->rfirMap[name] = rfir;         
    }

    return rfir;
}


