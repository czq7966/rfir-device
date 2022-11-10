#include "cmd.h"
#include "cmds/cmd/reg-table.h"
#include "rfir/util/debuger.h"
#include "rfir/util/sum.h"


void cmds::cmd::Cmd::reset(Head* head){
    if (head) {
        memset(head, 0, sizeof(Head));    
        head->pro_logo = PRO_LOGO;
    }
};  

uint16_t cmds::cmd::Cmd::getsum(){
    if (this->head)
        return rfir::util::Sum::Xor((uint8_t*)this->payload, this->head->pld_len);

    return 0;
};

bool cmds::cmd::Cmd::setsum(){
    if (this->head) {
        this->head->pld_sum = this->getsum();
        return true;
    }

    return 0;
};

bool cmds::cmd::Cmd::checksum(){
    if (this->head)
        return this->head->pld_sum == this->getsum();

    return 0;
};

 size_t cmds::cmd::Cmd::length(){
    return sizeof(Head) + this->head->pld_len;
 };

bool cmds::cmd::RecvCmd::recv(const char* buf, int size){
    this->params.buf = (char*)buf;
    this->params.bufsize = size;
    if (this->decode()) {
        this->events.recv.emit(this);
        return true;
    }  
    return false;
};

bool cmds::cmd::RecvCmd::decode(){
    this->reset();
    this->head = (Head*)this->params.buf;
    if (head->pro_logo == PRO_LOGO && this->head->pld_len + sizeof(Head) == this->params.bufsize){
        if (this->head->pld_len > 0) {
            this->payload = (char*)this->head + sizeof(Head);
            if(this->regTable.decode(this->payload, this->head->pld_len)){
                if (this->checksum())
                    return true;
                else 
                    return true;                
            };          
               
        } else return true;
    } else {
        GDebuger.print(F("cmds::cmd::RecvCmd::decode failed, pro_logo: "));
        GDebuger.print(head->pro_logo);
        GDebuger.print(F(" : "));
        GDebuger.print(PRO_LOGO);
        GDebuger.print(F(" , length:  "));
        GDebuger.print(this->head->pld_len + sizeof(Head));
        GDebuger.print(F(" : "));
        GDebuger.print(this->params.bufsize);
        GDebuger.println(F(" "));
        
    }
    return false;
};

void cmds::cmd::RecvCmd::reset(){
    this->regTable.clear();
};


//SendCmd
bool cmds::cmd::SendCmd::send(std::list<int> ids){
    for (auto it = ids.begin(); it != ids.end(); it++)
    {
        int key = *it;
        int value;
        if (!this->regTable.tables.exist(key)) {
            if (GRegTable.tables.get(key, value)) {
                this->regTable.tables.add(key, value);
                int len = 0;
                if (GRegTable.sizes.get(key, len))
                    this->regTable.sizes.add(key, len);
            }
        }
    }    
    
    return send();
};

bool cmds::cmd::SendCmd::send(){
    if (this->encode()) {
        int len = this->length(); 
        this->events.send.emit((void*)len);
        return true;
    }

    return false;

};

bool cmds::cmd::SendCmd::encode(){
    int len = 0;
    if (this->regTable.encode(this->payload, len)) {
        this->head->pld_len = (uint16_t)len;
        this->head->pld_sum = getsum();
        return true;
    };
    return false;
};

void cmds::cmd::SendCmd::reset(){
    this->regTable.clear();
    this->head = (Head*)this->params.buf;
    this->payload = (char*)this->head + sizeof(Head);
    Cmd::reset(this->head);
};

cmds::cmd::RecvCmd GRecvCmd;
cmds::cmd::SendCmd GSendCmd;