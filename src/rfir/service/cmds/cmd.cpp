#include "cmd.h"
#include "../../module/ttl/encoder.h"

rfir::module::cmds::Cmd* rfir::service::cmds::Cmd::cmd = new rfir::module::cmds::Cmd();


bool rfir::service::cmds::Cmd::onCmd(const char * cmdStr) {
    neb::CJsonObject cmd;
    if (cmd.Parse(cmdStr)) {
        onCmd(&cmd);
        return true;
    } else {
        Serial.println("not json");
    }
    return false;
}

bool rfir::service::cmds::Cmd::onCmd(neb::CJsonObject* jCmd) {
    neb::CJsonObject hd;
    neb::CJsonObject pld;
    if (jCmd->Get("hd", hd)) {
        int cmd = 0;
        if (hd.Get("cmd", cmd)) {
            if (!(jCmd->Get("pld", pld)))
                Serial.println("no pld");
            Serial.println("on Cmd: " + String(cmd));
            switch (cmd)
            {
                case 7:     //restart
                    return onCmd_restart(&hd, &pld);
                    break;                
                case 8:     //send  
                    return onCmd_send(&hd, &pld);
                    break;
                case 9:     //sniff        
                    return onCmd_sniff(&hd, &pld); 
                    break;                
                case 10:   //decode or encode                 
                    return onCmd_decode_encode(&hd, &pld); 
                    break;                                    
                case 11:
                    // return onCmd_config(&hd, &pld); 
                    break;                                    
                default:
                    break;
            }
        }
    }
    return false;
}

bool rfir::service::cmds::Cmd::onCmd_restart(neb::CJsonObject* hd, neb::CJsonObject* pld) {    
    ESP.restart();
    return true;
}

bool rfir::service::cmds::Cmd::onCmd_send(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    std::string name;
    pld->Get("device", name);
    auto device = rfir::getRfirDevice(name);
    if (device) {
        rfir::module::ttl::Config::Device d;
        d.clone(device);

        neb::CJsonObject jsend, jencode, jblocks;
        if (pld->Get("send", jsend)) {
            d.packet.send.parseFromJson(&jsend);

            std::string raw;
            if (jsend.Get("raw", raw) && raw.length() > 0) {
                onCmd_sendRaw(&d, raw.c_str(), raw.length());
            }
        }

        if (pld->Get("encode", jencode) && jencode.Get("blocks", jblocks)) {
            d.packet.encode.parseFromJson(&jblocks);
            onCmd_sendEncode(&d, &jencode);
        } 

        return true;

    } else {
        Serial.println(("onCmd_sniff->no config device:" + name).c_str());
    }

    return false;
}

bool rfir::service::cmds::Cmd::onCmd_send(rfir::module::ttl::Config::Device* d) {
    auto rfir = rfir::Get()->getRfir(d->getName());
    rfir->sender->getSendParams()->clone(&d->packet.send);
    rfir->encoder->getEncodeParams()->clone(&d->packet.encode);
    return true;
}

bool rfir::service::cmds::Cmd::onCmd_sendRaw(rfir::module::ttl::Config::Device* d, const char* data, int size) {
    auto rfir = rfir::Get()->getRfir(d->getName());    
    rfir->sender->name = d->getName();
    rfir->sender->getSendParams()->clone(&d->packet.send);
    rfir->sender->sendRaw(data, size);
    return true;
}

bool rfir::service::cmds::Cmd::onCmd_sendEncode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* jencode) {
    if (jencode) {
        neb::CJsonObject jblocks;
        if (jencode->Get("blocks", jblocks)) {
            d->packet.encode.parseFromJson(&jblocks);
            auto rfir = rfir::Get()->getRfir(d->getName());
            rfir->encoder->getEncodeParams()->clone(&d->packet.encode);
            if (rfir->encoder->encode(&jblocks)) {
                const char* data = rfir->encoder->toString();
                return onCmd_sendRaw(d, data, strlen(data));
            }
        } 
    }

    return false;
    
}

bool rfir::service::cmds::Cmd::onCmd_sniff(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    std::string name;
    pld->Get("device", name);
    auto device = rfir::getRfirDevice(name);
    if (device) {
        rfir::module::ttl::Config::Device d;
        d.clone(device);
        neb::CJsonObject jsniff, jp, jdecode, jblocks;
        if (pld->Get("sniff", jsniff)) 
            d.packet.sniff.parseFromJson(&jsniff);

        if (pld->Get("decode", jdecode)) {
            d.packet.decode.parseFromJson(&jdecode);
        } 

        return onCmd_sniff(&d);
    } 
    return false;
}

bool rfir::service::cmds::Cmd::onCmd_sniff(rfir::module::ttl::Config::Device* d, bool start) {
    auto rfir = rfir::Get()->getRfir(d->getName());
    rfir->decoder->name = d->getName();
    rfir->decoder->getDecodeParams()->clone(&d->packet.decode);
    rfir->decoder->initDecodeResults();
    rfir->encoder->name = d->getName();
    rfir->encoder->getEncodeParams()->clone(rfir->decoder->getDecodeParams());
    rfir->sniffer->name = d->getName();
    rfir->sniffer->stop();
    rfir->sniffer->init(d->packet.sniff);
    if (start) {
        rfir->sniffer->start();        
    }

    return true;
}

bool rfir::service::cmds::Cmd::onCmd_decode_encode(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    std::string name;
    pld->Get("device", name);
    auto device = rfir::getRfirDevice(name);
    if (device) {
        rfir::module::ttl::Config::Device d;
        d.clone(device);

        neb::CJsonObject jdecode, jencode;
        if (pld->Get("decode", jdecode))
            onCmd_decode(&d, &jdecode);
        
        if (pld->Get("encode", jencode))
            onCmd_encode(&d, &jencode);

        return true;
    }
    return false;
}

bool rfir::service::cmds::Cmd::onCmd_decode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* jdecode) {
    auto rfir = rfir::Get()->getRfir(d->getName());
    rfir->decoder->name = d->getName();
    std::string data;
    if (jdecode->Get("raw", data)) {
        int count = 0;
        uint16_t* raw  = rfir::module::ttl::Encoder::parseRaw(data.c_str(), data.length(), count);
        rfir->decoder->getDecodeParams()->clone(&d->packet.decode);
        rfir->decoder->getDecodeParams()->parseFromJson(jdecode);

        bool result = rfir->decoder->decode(raw, count);
        if (!result)
            Serial.println("onCmd_decode failed: 无符合格式的数据");
        
        delete raw; 

        return result;
    } else {
        Serial.println("onCmd_decode failed: 没有原始数据 ");
    }
    return false;
}
bool rfir::service::cmds::Cmd::onCmd_encode(rfir::module::ttl::Config::Device* d, neb::CJsonObject* jencode) {
    neb::CJsonObject jblocks;
    if (jencode->Get("blocks", jblocks)) {
        auto rfir = rfir::Get()->getRfir(d->getName());
        rfir->encoder->name = d->getName();
        rfir->encoder->getEncodeParams()->clone(&d->packet.encode);        
        rfir->encoder->getEncodeParams()->parseFromJson(jencode);
        bool result = rfir->encoder->encode(&jblocks);
        if (!result)
            Serial.println("onCmd_encode failed");
        
        return result;
    } else {
        Serial.println("onCmd_encode failed: 没有编码数据");
    }
    return false;
}


bool rfir::service::cmds::Cmd::onCmd_config(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    neb::CJsonObject jcontent;
    int mode = -1;
    pld->Get("mode", mode);
    if (mode == 0) {
        if (pld->Get("content", jcontent))
            return onCmd_config_change(&jcontent);
    }

    if (mode == 1) {
        if (pld->Get("content", jcontent))
            return onCmd_config_save(&jcontent);
    }    

    if (mode == 2) {
        return onCmd_config_delete();
    }    

    return false;
}

bool rfir::service::cmds::Cmd::onCmd_config_change(neb::CJsonObject* jcontent) {
    return rfir::RFIR::RefreshConfig(jcontent->ToString().c_str());
}

bool rfir::service::cmds::Cmd::onCmd_config_save(neb::CJsonObject* jcontent) {
    if (jcontent) {
        rfir::util::TxtFile file(rfir::RFIR::ConfigFile.c_str());
        std::string content = jcontent->ToString();
        file.writeString(content.c_str(), content.length());
        return true;
    }
    return false;
}

bool rfir::service::cmds::Cmd::onCmd_config_delete() {   
    rfir::RFIR::Stop();
    rfir::util::File::remove(rfir::RFIR::ConfigFile.c_str());
    rfir::RFIR::RefreshConfig();
    rfir::RFIR::Start();
    return true;
}


