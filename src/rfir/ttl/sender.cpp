#include "sender.h"
#include "ir-send.h"
#include "rfir/util/timer.h"
#include "rfir/util/platform.h"


void rfir::ttl::Sender::sendRaw(const char* data, const int size){
    std::list<uint16_t> deltas;
    std::string str;
    for (size_t i = 0; i < size; i++)
    {    
        char c = data[i];
        if (c != ',' ) 
            str += c;
        else  {    
            deltas.push_back(std::atoi(str.c_str()));
            str = "";
        }
    }
    if (str != "")
        deltas.push_back(std::atoi(str.c_str()));

    sendRaw(deltas);
};

void rfir::ttl::Sender::sendRaw(const uint16_t* data, const int size){
    std::list<uint16_t> deltas;
    for (size_t i = 0; i < size; i++)
    {   
        deltas.push_back(data[i]);
    }

    sendRaw(deltas);
};

void rfir::ttl::Sender::sendRaw(std::list<uint16_t>& deltas){
    if (!params.enabled || deltas.size() == 0)
        return;

    bool inverted = params.inverted;
    if (*deltas.begin() == 1)
        inverted = !inverted;

    IRSend irsend(params.pin, inverted, params.modulation);
    irsend.begin();
    irsend.enableIROut(params.frequency);

    for (size_t r = 0; r <= params.repeat; r++)
    {
        uint16_t i = 0;
        for (auto it = deltas.begin(); it != deltas.end(); it++)
        {
            if (i & 1) { 
                irsend.space(*it);
            } else {  
                irsend.mark(*it);
            }
            i++;
        }
    }
    
    irsend.space(0);  
};

rfir::ttl::Sender GTTLSender;