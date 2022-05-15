#include "sender-v2.h"
#include "rfir/util/timer.h"
#include "ir-send.h"


std::string rfir::module::ttl::SenderV2::Params::toString(){
    char result[128] = {0};
    sprintf(result, "pin: %d, inverted: %d, modulation: %d, repeat: %d, frequency: %d, dutycycle: %d",
            pin, inverted, modulation, repeat, frequency, dutycycle);
    return result;
};

void rfir::module::ttl::SenderV2::sendRaw(const char* data, const int size){
    std::list<uint16_t> deltas;
    std::string str;
    for (size_t i = 0; i < size; i++)
    {    
        char c = data[i];
        if (c != ',' ) 
            str += c;
        else  {    
            deltas.push_back(atoi(str.c_str()));
            str = "";
        }
    }
    if (str != "")
        deltas.push_back(atoi(str.c_str()));

    sendRaw(deltas);
};

void rfir::module::ttl::SenderV2::sendRaw(std::list<uint16_t>& deltas){
    if (deltas.size() == 0)
        return;

    bool inverted = params.inverted;
    if (*deltas.begin() == 1)
        inverted = !inverted;

    IRSend irsend(params.pin, inverted, params.modulation);
    irsend.begin();
    irsend.enableIROut(params.frequency);

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
    
    irsend.space(0);  
};

rfir::module::ttl::SenderV2 GTTLSender;