#include "sender.h"


rfir::module::serial::Sender* rfir::service::serial::Sender::sender = new rfir::module::serial::Sender();

void rfir::service::serial::Sender::dealCmd() {
    sender->dealCmd();
}

bool rfir::service::serial::Sender::pushCmd(neb::CJsonObject* cmd){
    return sender->pushCmd(cmd);
}

bool rfir::service::serial::Sender::pushCmd(const char* topic, const char* payload) {
    neb::CJsonObject* cmd = new neb::CJsonObject();
    cmd->Add("topic", topic);
    cmd->Add("payload", payload);
    pushCmd(cmd);
    
}

bool rfir::service::serial::Sender::popCmd(neb::CJsonObject* cmd){
    return sender->popCmd(cmd);
}


bool rfir::service::serial::Sender::error(const char* msg) {
    neb::CJsonObject* cmd = new neb::CJsonObject();
    neb::CJsonObject hd, pld;
    cmd->AddEmptySubObject("hd");
    cmd->AddEmptySubObject("pld");
    return false;
}

