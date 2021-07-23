#include "cmd.h"
#include "../ttl/sniffer.h"
#include "../ttl/sender.h"
#include "../ttl/decoder.h"
#include "../ttl/config.h"
#include "../../module/ttl/sniffer.h"
#include "../../module/ttl/encoder.h"

#include "IRsend.h"

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

void rfir::service::cmds::Cmd::onCmd(neb::CJsonObject* jCmd) {
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
                case 8:       
                    onCmd_send(&hd, &pld);
                    break;
                case 9:                   
                    onCmd_sniff(&hd, &pld); 
                    break;                
                case 10:                    
                    onCmd_decode(&hd, &pld); 
                    break;                                    
                default:
                    break;
            }
        }
    }
}

void rfir::service::cmds::Cmd::onCmd_send(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    rfir::module::ttl::Sender::Params p = rfir::service::ttl::Sender::ttlsender->params;
    neb::CJsonObject jsend, jp;
    if (pld->Get("send", jsend) ) {
        if (jsend.Get("params", jp))
            rfir::module::ttl::Sender::parseParams(&jp, &p);
        std::string raw;
        neb::CJsonObject jraw;
        if (jsend.Get("raw", raw)) {
            onCmd_send(p, raw.c_str(), raw.length());            
        } else {
            if (jsend.Get("raw", jraw)) {

            }
        }
    } else {
        Serial.println("no send pld");
    }
}

void rfir::service::cmds::Cmd::onCmd_send(rfir::module::ttl::Sender::Params p, const char* data, int size) {
    int count;
    uint16_t* raw  = rfir::module::ttl::Encoder::parseRaw(data, size, count);        


    IRsend irsend(p.pin, p.inverted, p.modulation);
    irsend.begin();
    for (size_t i = 0; i <= p.repeat; i++)
    {
        irsend.sendRaw(raw, count, 38);
    }
    
    



    // rfir::service::ttl::Sender::ttlsender->init(p);
    // rfir::service::ttl::Sender::ttlsender->sendRaw(raw, count);

    delete raw;
}

void rfir::service::cmds::Cmd::onCmd_sniff(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    rfir::module::ttl::Sniffer::Params p = rfir::service::ttl::Sniffer::ttlsniffer->params;
    neb::CJsonObject jsniff, jp, jdecode;
    if (pld->Get("sniff", jsniff) && jsniff.Get("params", jp)) {
        rfir::module::ttl::Sniffer::parseParams(&jp, &p);
    } else {
        Serial.println("no params");
    }

    if (pld->Get("decode", jdecode) ) {
        rfir::service::ttl::Decoder::ttldecoder->setJDecode(&jdecode);        
    } else {
        Serial.println("no decode");
    }

    onCmd_sniff(p);    
}

void rfir::service::cmds::Cmd::onCmd_sniff(rfir::module::ttl::Sniffer::Params p) {
    rfir::service::ttl::Sniffer::stop();
    rfir::service::ttl::Sniffer::init(p);
    rfir::service::ttl::Sniffer::start();
    Serial.println(p.toString().c_str());
    Serial.println("start sniff......");
}

void rfir::service::cmds::Cmd::onCmd_decode(neb::CJsonObject* hd, neb::CJsonObject* pld) {
    rfir::module::ttl::Sniffer::Params p = rfir::service::ttl::Sniffer::ttlsniffer->params;
    neb::CJsonObject jsniff, jp;
    if (pld->Get("sniff", jsniff) && jsniff.Get("params", jp)) {
        rfir::module::ttl::Sniffer::parseParams(&jp, &p);
    } else {
        Serial.println("no params");
    }
}