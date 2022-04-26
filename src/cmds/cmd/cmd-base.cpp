#include "cmd-base.h"
#include "rfir/util/event-timer.h"
#include "config.h"

uint32_t& cmds::cmd::CmdBase::Command::SessionID = rfir::util::EventTimer::HandlerIndex;

uint32_t cmds::cmd::CmdBase::Command::getSid(){
    return cache.sid;
};
bool cmds::cmd::CmdBase::Command::setSid(uint32_t value){
    cache.sid = value;
    return 1;
};
bool cmds::cmd::CmdBase::Command::isReqCmd(){
    return cache.stp == 0;
};
bool cmds::cmd::CmdBase::Command::isRespCmd(){
    return cache.stp == 1;
};
bool cmds::cmd::CmdBase::Command::setReqCmd(){
    cache.stp = 0;
    return 1;
};
bool cmds::cmd::CmdBase::Command::setRespCmd(){
    cache.stp = 1;
    return 1;
};

bool cmds::cmd::CmdBase::Command::isNeedResp(){
    return isReqCmd() && getSid() > 0;
};
bool cmds::cmd::CmdBase::Command::setNeedResp(bool value){
    if (value) {
        SessionID++;
        setSid(SessionID);
    } else {
        setSid(0);
    }
    return 1;
};

void cmds::cmd::CmdBase::Command::cloneFrom(Command& cmd){
    cache = cmd.cache;
    cloneFrom(cmd.hd, cmd.pld);
};
void cmds::cmd::CmdBase::Command::cloneTo(Command& cmd){
    cmd.cache = cache;
    cloneTo(cmd.hd, cmd.pld);
};
void cmds::cmd::CmdBase::Command::cloneFrom(neb::CJsonObject& cmd) {
    cmd.Get("hd", hd);
    cmd.Get("pld", pld);
    hd.Get("sid", cache.sid);
    hd.Get("stp", cache.stp);
};
void cmds::cmd::CmdBase::Command::cloneTo(neb::CJsonObject& cmd){
    hd.ReplaceAdd("sid", cache.sid);
    hd.ReplaceAdd("stp", cache.stp);
    cmd.ReplaceAdd("hd", hd);
    cmd.ReplaceAdd("pld", pld);
};

void cmds::cmd::CmdBase::Command::cloneFrom(neb::CJsonObject& _hd, neb::CJsonObject& _pld){
    hd.Parse(_hd.ToString());
    pld.Parse(_pld.ToString());
};
void cmds::cmd::CmdBase::Command::cloneTo(neb::CJsonObject& _hd, neb::CJsonObject& _pld){
    _hd.Parse(hd.ToString());
    _pld.Parse(pld.ToString());
};

std::string cmds::cmd::CmdBase::Command::toString(){
    neb::CJsonObject cmd;
    cloneTo(cmd);
    return cmd.ToString();
};

void cmds::cmd::CmdBase::Command::fromString(const char* str) {
    neb::CJsonObject cmd(str);
    cloneFrom(cmd);
}

void cmds::cmd::CmdBase::Events::cloneFrom(Events& events){
    onResp = events.onResp;
    onTimeout = events.onTimeout;

};
void cmds::cmd::CmdBase::Events::cloneTo(Events& events){
    events.onResp = onResp;
    events.onTimeout = onTimeout;
};     

cmds::cmd::CmdBase::Events* cmds::cmd::CmdBase::Events::clone(){
    auto events = new Events();
    cloneTo(*events);
    return events;
};

bool cmds::cmd::CmdBase::send(const void* p){
    return false;
};
void cmds::cmd::CmdBase::cloneFrom(CmdBase& cmd){
    command.cloneFrom(cmd.command);
    events.cloneFrom(cmd.events);
};
void cmds::cmd::CmdBase::cloneTo(CmdBase& cmd){
    command.cloneTo(cmd.command);
    events.cloneTo(cmd.events);
};   

void* cmds::cmd::CmdBase::clone() {
    auto cmd = new CmdBase();
    this->cloneTo(*cmd);
    return (void*)cmd;
}; 