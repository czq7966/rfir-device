#include "cmd.h"
#include "cmds/cmd/reg-table.h"

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
    this->head = (Head*)this->params.buf;
    if (this->head->pld_len + sizeof(Head) == this->params.bufsize){
        this->payload = (char*)this->head + sizeof(Head);
        return this->regTable.decode(this->payload, this->head->pld_len);
    };
    return false;
};

void cmds::cmd::RecvCmd::reset(){

};

//SendCmd
bool cmds::cmd::SendCmd::send(std::list<int> ids){
    for (auto it = ids.begin(); it != ids.end(); it++)
    {
        int key = *it;
        int value;
        if (!this->regTable.tables.get(key)) {
            if (GRegTable.tables.get(key, value)) {
                this->regTable.tables.add(key, value);
            }
        }
    }    
    
    return send();
};

bool cmds::cmd::SendCmd::send(){
    if (this->encode()) {
        int len = sizeof(Head) + this->head->pld_len;
        this->events.send.emit((void*)len);
        return true;
    }

    return false;

};

bool cmds::cmd::SendCmd::encode(){
    int len = 0;
    if (this->regTable.encode(this->payload, len)) {
        this->head->pld_len = (uint16_t)len;
        return true;
    };
    return false;
};

void cmds::cmd::SendCmd::reset(){
    this->regTable.tables.clear();
    this->head = (Head*)this->params.buf;
    this->payload = (char*)this->head + sizeof(Head);
    memset(this->head, 0, sizeof(Head));
    this->head->pro_logo = PRO_LOGO;
};

cmds::cmd::RecvCmd GRecvCmd;
cmds::cmd::SendCmd GSendCmd;